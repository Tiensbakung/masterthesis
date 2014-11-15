#include <sstream>
#include <algorithm>
#include "common.h"
#include "utils.h"
#include "logging.h"
using namespace std;


class Browser: public cSimpleModule
{
private:
        ipaddr addr;
        ipaddr gateWayIP;
        cModule *parent;
        vSegment realSegments;
        vIndex currentVideoIndex;
        vSegment totalSemgents;
        cMessage *requestEvent;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage*);
        void sendSessionCloseNotfiy(ipaddr dst, vIndex i, vSegment s);
        void setupConnection(ipaddr, vIndex, vSegment);
        void teardownConnection();
        void terminateConnection(ipaddr dst, vIndex i, vSegment s);
        void requestVideoSegement(ipaddr, vIndex, vSegment);
        void requestNewVideo();
        void scheduleRequestEvent();
public:
        Browser() : cSimpleModule() {
                requestEvent = NULL;
        }
        virtual ~Browser() {
                cancelAndDelete(requestEvent);
        }
};

Define_Module(Browser);

void Browser::initialize()
{
        parent = getParentModule();
        gateWayIP = parent->par("gateway");
        addr = parent->getIndex() + gateWayIP + 1;
        realSegments = 0;
        currentVideoIndex = 0;
        totalSemgents = 0;
        requestEvent = new cMessage("requestEvent");
        scheduleRequestEvent();
}

void Browser::handleMessage(cMessage* msg)
{
        if (!strcmp(msg->getName(), "packet")) {
                Paket *p = check_and_cast<Paket*>(msg);
                vSequence v = (vSequence)p->getV();
                if (v == 0 || v+1 == SEQNUMBER) {
                        vSegment s = p->getSeg();
                        logging(LOG_PKT, addr, p->getSrc(), p->getIndex(), s, v);
                }
                delete p;
        } else if (!strcmp(msg->getName(), "teardown")) {
                Paket *p = check_and_cast<Paket*>(msg);
                ipaddr dst = p->getSrc();
                vIndex i = p->getIndex();
                vSegment s = p->getSeg()+1;
                if (s < realSegments) {
                        requestVideoSegement(dst, i, s);
                        logging(LOG_REQ, addr, dst, i, s);
                } else if (s == realSegments) {
                        // terminateConnection(dst, i, s);
                        sendSessionCloseNotfiy(dst, i, s-1);
                        logging(LOG_TDN, addr, dst, i, s-1);
                        teardownConnection();
                }
                delete p;
        } else if (msg == requestEvent) {
                requestNewVideo();
        } else if (!strcmp(msg->getName(), "video_info")) {
                Paket *p = check_and_cast<Paket*>(msg);
                ipaddr dst = (ipaddr)p->getV();
                vIndex i = p->getIndex();
                setupConnection(dst, i, p->getSeg());
                scheduleRequestEvent();
                logging(LOG_STP, addr, dst, i, p->getSeg());
                requestVideoSegement(dst, i, 0);
                logging(LOG_REQ, addr, dst, i, 0);
                delete p;
        }
}

void Browser::requestNewVideo()
{
        size_t i = 0;
        Paket *request = generateNewVideoRequest(addr, gateWayIP, i);
        send(request, "port$o");
}

void Browser::setupConnection(ipaddr dst, vIndex i, vSegment s)
{
        currentVideoIndex = i;
        totalSemgents = s;
        //for (realSegments = zipf(); realSegments >= 4500;
        //     realSegments=zipf())
        realSegments = min(kzipf()+1, s);
        //realSegments = min(realSegments+1, s);
}

void Browser::sendSessionCloseNotfiy(ipaddr dst, vIndex i, vSegment s)
{
        Paket *p = generateSessionCloseNotify(addr, dst, i, s);
        send(p, "port$o");
}

void Browser::terminateConnection(ipaddr dst, vIndex i, vSegment s)
{
        Paket *termination = generateTermination(addr, dst, i, s);
        send(termination, "port$o");
        teardownConnection();
}

void Browser::teardownConnection()
{
        realSegments = 0;
        currentVideoIndex = 0;
        totalSemgents = 0;
}

void Browser::requestVideoSegement(ipaddr dst, vIndex i, vSegment s)
{
        Paket *request = generateSegmentRequest(addr, dst, i, s);
        send(request, "port$o");
}

void Browser::scheduleRequestEvent()
{
        simtime_t delta = poisson(30) + realSegments*10;
        if (!requestEvent->isScheduled()) {
                scheduleAt(simTime()+delta, requestEvent);
        }
}
