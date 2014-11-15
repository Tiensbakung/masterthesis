#include "common.h"
#include "video.h"
#include "filesystem.h"
#include "server.h"
#include "iptools.h"
#include "logging.h"
using namespace std;

class RegionServer : public Server
{
private:
        bool isReportStats;
        int reportInterval;
        cMessage *reportEvent;
        Stats stats;
protected:
        void handleVideoOperation(vIndex, vSegment, cValue);
        void handlePrecachingHint(vIndex i, vSegment s);
        void reportStats();
        void scheduleReportEvent();
        void sendSegmentHint(vIndex i, vSegment s);
        virtual void initialize();
        virtual void handleMessage(cMessage* msg);
public:
        RegionServer() : Server(), reportEvent(NULL) {}
        virtual ~RegionServer() {cancelAndDelete(reportEvent);}
};

Define_Module(RegionServer);

void RegionServer::initialize()
{
        Server::initialize();
        reportInterval = par("reportInterval");
        reportEvent = new cMessage("reportEvent");
        isReportStats = par("reportStats").boolValue();
        if (isReportStats) {
                scheduleReportEvent();
        }
}

void RegionServer::handleMessage(cMessage *msg)
{
        if (msg == reportEvent) {
                reportStats();
                stats.clear();
                scheduleReportEvent();
        } else if (!strcmp(msg->getName(), "request")) {
                Paket *p = check_and_cast<Paket*>(msg);
                if (isReportStats && is_host(p->getSrc())) {
                        vIndex i = p->getIndex();
                        vSegment s = p->getSeg();
                        stats[VideoSegment(i, s)]++;
                        if (!FileSystem::instance().exists(addr, i)) {
                                sendSegmentHint(i, s);
                                // logging(LOG_SSH, addr, centralServerIP, i, s);
                        }
                }
                Server::handleMessage(p);
        } else if (!strcmp(msg->getName(), "precaching_hint")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                handlePrecachingHint(i, p->getSeg());
                // logging(LOG_PCH, addr, p->getSrc(), i, p->getSeg());
                delete p;
        } else if (!strcmp(msg->getName(), "operation")) {
                Paket *p = check_and_cast<Paket*>(msg);
                vIndex i = p->getIndex();
                
                handleVideoOperation(i, p->getSeg(), p->getV());
                if (p->getV() == VIDEO_DROP) {
                        logging(LOG_OPE, addr, p->getSrc(), i, 0, p->getV());
                }
                delete p;
        } else {
                Server::handleMessage(msg);
        }
}

void RegionServer::handlePrecachingHint(vIndex i, vSegment s)
{
        VideoSegment e(i, s);
        if (cache.exists(e)) {
                cache.get(e);
                logging(LOG_HHC, addr, centralServerIP, i, s);
        } else {
                queryVideoSegment(i, s);
                logging(LOG_HNH, addr, centralServerIP, i, s);
                logging(LOG_QRY, addr, centralServerIP, i, s);
        }
}

void RegionServer::handleVideoOperation(vIndex i, vSegment s, cValue v)
{
        VideoSegment e(i, s);
        if (v == VIDEO_DROP) {
                cache.remove(e);
                logging(LOG_CLN, addr, 0, cache.size());
        } else if (v == VIDEO_LIFT) {
                if (FileSystem::instance().exists(addr, i)) {
                        logging(LOG_OHD, addr, centralServerIP, i, s);
                } else if (cache.exists(e)) {
                        cache.get(e);
                        logging(LOG_OHC, addr, centralServerIP, i, s);
                } else {
                        queryVideoSegment(i, s);
                        logging(LOG_ONH, addr, centralServerIP, i, s);
                        logging(LOG_QRY, addr, centralServerIP, i, s);
                }
        }
}

void RegionServer::sendSegmentHint(vIndex i, vSegment s)
{
        Paket *p = generateSegmentHint(addr, centralServerIP, i, s);
        send(p, "port$o");
}

void RegionServer::scheduleReportEvent()
{
        setTimer(reportEvent, reportInterval);
}

void RegionServer::reportStats()
{
        Report *report = generateReport(addr, centralServerIP, stats);
        send(report, "port$o");
}
