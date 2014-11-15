#include "common.h"
#include "route.h"
#include "iptools.h"
using namespace std;

class GateRoute : public Route
{
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
public:
        GateRoute() : Route() {}
        virtual ~GateRoute() {}
};

Define_Module(GateRoute);

void GateRoute::initialize()
{
        Route::initialize();
}

void GateRoute::handleMessage(cMessage *msg)
{
        ipaddr dst = 0;
        if (!strcmp(msg->getName(), "report")) {
                Report *p = check_and_cast<Report *>(msg);
                dst = p->getDst();
        } else {
                Paket *p = check_and_cast<Paket*>(msg);
                dst = p->getDst();
        }
        if (addr == dst) {
                send(msg, "localPort$o");
        } else if (ip_subnet(addr) == ip_subnet(dst)) { // LAN IP address
                port v = ip_number(dst)-1;
                send(msg, "lan$o", v);
        } else {
                Route::handleMessage(msg);
        }
}
