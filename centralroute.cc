#include "common.h"
#include "route.h"
using namespace std;

class CentralRoute : public Route
{
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
public:
        CentralRoute() : Route() {}
        virtual ~CentralRoute() {}
};

Define_Module(CentralRoute);

void CentralRoute::initialize()
{
        Route::initialize();
}

void CentralRoute::handleMessage(cMessage *msg)
{
        ipaddr dst = 0;
        if (!strcmp(msg->getName(), "report")) {
                Report *p = check_and_cast<Report *>(msg);
                dst = p->getDst();
        } else {
                Paket *p = check_and_cast<Paket*>(msg);
                dst = p->getDst();
        }
        if (dst == addr && !strcmp(msg->getName(), "query")) {
                send(msg, "extraPort$o");
        } else if (dst == addr && !strcmp(msg->getName(), "segment_hint")) {
                send(msg, "extraPort$o");
        } else if (dst == addr && !strcmp(msg->getName(), "report")) {
                send(msg, "extraPort$o");
        } else {
                Route::handleMessage(msg);
        }
}
