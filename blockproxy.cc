#include "filesystem.h"
#include "blockproxy.h"
#include "logging.h"


Define_Module(BlockProxy);

void BlockProxy::initialize()
{
        Server::initialize();
        serverNumbers = parent->par("numberOfServers");
        setRandomServer();
}

void BlockProxy::handleMessage(cMessage *msg)
{
        if (!strcmp(msg->getName(), "query")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                vSegment s = p->getSeg();
                if (exists(i, s)) {
                        replySegmentQuery(p->getSrc(), i, s, addr);
                        logging(LOG_REP, addr, p->getSrc(), i, s, addr);
                } else {
                        replyNotFound(p->getSrc(), i, s);
                        logging(LOG_NFD, addr, p->getSrc(), i, p->getSeg());
                }
                delete p;
        } else if (!strcmp(msg->getName(), "not_found")) {
                Paket *p = check_and_cast<Paket *>(msg);
                setRandomServer();
                vIndex i = p->getIndex();
                queryVideoSegment(i, p->getSeg());
                logging(LOG_QRY, addr, centralServerIP, i, p->getSeg());
                delete p;
        } else {
                Server::handleMessage(msg);
        }
}

void BlockProxy::replySegmentQuery(ipaddr dst, vIndex i, vSegment s,
        cValue v)
{
        Paket *reply = generateReply(addr, dst, i, s, v);
        send(reply, "port$o");
}

void BlockProxy::replyNotFound(ipaddr dst, vIndex i, vSegment s)
{
        Paket *reply = generateNotFound(addr, dst, i, s);
        send(reply, "port$o");
}

void BlockProxy::setRandomServer()
{
        ipaddr newIP = intuniform(0, serverNumbers-1);
        while (newIP == addr || newIP == centralServerIP) {
                newIP = intuniform(0, serverNumbers-1);
        }
        centralServerIP = newIP;
}
