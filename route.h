#ifndef _ROUTE_H_
#define _ROUTE_H_
#include "paket_m.h"
#include "report_m.h"

typedef size_t port;
typedef std::unordered_map<ipaddr, port> RouteTable;

class Route: public cSimpleModule
{
protected:
        cModule *parent;
        ipaddr addr;
        RouteTable routeTable;
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
public:
        Route();
        virtual ~Route();
};

#endif  /* _ROUTE_H_ */
