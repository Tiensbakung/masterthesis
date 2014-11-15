#include <cstring>
#include "common.h"
#include "server.h"
#include "filesystem.h"
#include "iptools.h"
#include "logging.h"
using namespace std;


Define_Module(Server);

void Server::initialize()
{
        parent = getParentModule();
        addr = parent->par("address");
        centralServerIP = parent->par("centralServerIP");
        handleRequestEvent = new cMessage("handleRequestEvent");
}

void Server::handleMessage(cMessage *msg)
{
        if (msg == handleRequestEvent) {
                handleRequests();
                if (!connTable.empty())
                        scheduleRequestEvent();
        } else if (!strcmp(msg->getName(), "packet")) {
                Paket *p = check_and_cast<Paket*>(msg);
                vIndex i = p->getIndex();
                vSegment s = p->getSeg();
                vSequence v = p->getV();
                if (v == 0 || v+1 == SEQNUMBER) {
                        logging(LOG_PKT, addr, p->getSrc(), i, s, v);
                }
                VideoSegment e(i, s);
                cache.put(e, v);
                delete p;
        } else if (!strcmp(msg->getName(), "request")) {
                Paket *p = check_and_cast<Paket*>(msg);
                setupConnection(p->getSrc(), p->getIndex(), p->getSeg());
                if (!handleRequestEvent->isScheduled())
                        scheduleRequestEvent();
                // logging(LOG_STP, addr, p->getSrc(), i, s);
                delete p;
        } else if (!strcmp(msg->getName(), "reply")) {
                Paket *p = check_and_cast<Paket*>(msg);
                vIndex i = p->getIndex();
                vSegment s = p->getSeg();
                if (!exists(i, s)) {
                        ipaddr dst = (ipaddr)p->getV();
                        requestVideoSegement(dst, i, s);
                        logging(LOG_REQ, addr, dst, i, s);
                }
                delete p;
        } else if (!strcmp(msg->getName(), "close")) {
                delete msg;
        // } else if (!strcmp(msg->getName(), "termination")) {
        //         Paket *p = check_and_cast<Paket*>(msg);
        //         ipaddr dst = p->getSrc();
        //         teardownConnection(dst, p->getIndex(), p->getSeg());
        //         sendTearDownNotify(addr, p->getIndex(), p->getSeg());
        //         logging(LOG_TDN, addr, dst, p->getIndex(), p->getSeg());
        //         delete p;
        } else {
                EV << "[ERR] Unexpected message: " << msg->getName() << endl;
                delete msg;
        }
}

bool Server::exists(vIndex i, vSegment s) const
{
        return FileSystem::instance().exists(addr, i) ||
                cache.exists(VideoSegment(i, s));
}

void Server::setupConnection(ipaddr c, vIndex i, vSegment s)
{
        Connection conn(c, i, s);
        VideoSegment e(i, s);
        ConnType t;
        if (FileSystem::instance().exists(addr, i)) {
                t = LOCAL;
                logging(LOG_RHD, addr, c, i, s);
        } else if (cache.exists(e)) {
                t = CACHED;
                logging(LOG_RHC, addr, c, i, s);
        } else {
                t = NEW;
                queryVideoSegment(i, s);
                logging(LOG_RNH, addr, c, i, s);
                logging(LOG_QRY, addr, centralServerIP, i, s);
        }

        auto it = connServ.find(conn);
        if (it == connServ.end()) {
                connTable.emplace_back(c, i, s, 0, t);
                connServ[conn] = prev(connTable.end());
        } else {
                connTable.splice(connTable.end(), connTable, it->second);
                it->second->v = 0;
                it->second->t = t;
                logging(LOG_RST, addr, c, i, s);
        }
}

void Server::teardownConnection(ipaddr c, vIndex i, vSegment s)
{
        auto it = connServ.find(Connection(c, i, s));
        if (it != connServ.end()) {
                connTable.erase(it->second);
                connServ.erase(it);
        } else {
                cout << "[Err] Unexpcted termination from " << IP(c) << endl;
        }
}

void Server::queryVideoSegment(vIndex i, vSegment s)
{
        Paket *query = generateQuery(addr, centralServerIP, i, s);
        send(query, "port$o");
}

void Server::requestVideoSegement(ipaddr dst, vIndex i, vSegment s)
{
        Paket *request = generateSegmentRequest(addr, dst, i, s);
        send(request, "port$o");
}

void Server::sendTearDownNotify(ipaddr dst, vIndex i, vSegment s)
{
        Paket *p = generateTearDownNotify(addr, dst, i, s);
        send(p, "port$o");
}

void Server::servRequest(ipaddr dst, vIndex i, vSegment s, cValue v)
{
        Paket* p = generateNetworkPacket(addr, dst, i, s, v);
        send(p, "port$o");
}

void Server::handleRequests()
{
        for (auto it = connTable.begin(); it != connTable.end();) {
                ipaddr dst = it->c;
                vIndex i = it->i;
                vSegment s = it->s;
                vSequence& v = it->v;
                if (it->t == LOCAL) {
                        servRequest(dst, i, s, v++);
                } else if (it->t == CACHED) {
                        try {
                                VideoSegment e(i, s);
                                const vSequence& seq = cache.get(e);
                                if (v <= seq) {
                                        servRequest(dst, i, s, v++);
                                        // logging(LOG_SPH, addr, dst, i, s, v);
                                } else {
                                        logging(LOG_SPM, addr, dst, i, s, v);
                                }
                        } catch (const range_error& e) {
                                it->t = NEW;
                                queryVideoSegment(i, s);
                                logging(LOG_RCM, addr, dst, i, s);
                                logging(LOG_QRY, addr, dst, i, s);
                        }
                } else if (it->t == NEW) {
                        if (cache.exists(VideoSegment(i, s))) {
                                it->t = CACHED;
                                // logging(LOG_NCS, addr, dst, i, s);
                        }
                }
                ++it;
                if (v >= SEQNUMBER) {
                        teardownConnection(dst, i, s);
                        sendTearDownNotify(dst, i, s);
                        logging(LOG_TDN, addr, dst, i, s);
                }
        }
}

void Server::setTimer(cMessage* msg, simtime_t delta)
{
        scheduleAt(simTime()+delta, msg);
}

void Server::scheduleRequestEvent()
{
        setTimer(handleRequestEvent, 0.02);
}
