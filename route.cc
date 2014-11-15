#include <cstring>
#include <cstdlib>
#include "common.h"
#include "route.h"
#include "iptools.h"
using namespace std;


Define_Module(Route);

Route::Route() : cSimpleModule(), routeTable()
{
        parent = NULL;
}

Route::~Route()
{
}

void Route::initialize()
{
        cXMLElement *root = par("table").xmlValue();
		for (cXMLElement *child = root->getFirstChild(); child;
             child=child->getNextSibling()) {
                ipaddr k = atoi(child->getAttribute("dest"));
                port v = atoi(child->getAttribute("port"));
                routeTable[k] = v;
        }
        parent = getParentModule();
        addr = parent->par("address");
        // EV << parent->getFullName() << ":\n" ;
        // for (auto it=routeTable.begin(); it!=routeTable.end(); ++it) {
        //         EV << "  " << it->first << ", " << it->second << endl;
        // }
}

void Route::handleMessage(cMessage *msg)
{
        ipaddr dst = 0;
        if (!strcmp(msg->getName(), "report")) {
                Report *p = check_and_cast<Report *>(msg);
                dst = p->getDst();
        } else {
                Paket *p = check_and_cast<Paket*>(msg);
                dst = p->getDst();
        }
        if (dst == addr)
                send(msg, "localPort$o");
        else {
                port v=0;
                if (is_server(dst))
                        v = routeTable[dst];
                else
                        v = routeTable[ip_subnet(dst)];
                send(msg, "line$o", v);
        }
}
