#ifndef _BLOCKPROXY_H
#define _BLOCKPROXY_H
#include "server.h"


class BlockProxy : public Server
{
private:
        int serverNumbers;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void replySegmentQuery(ipaddr, vIndex, vSegment, cValue);
        void replyNotFound(ipaddr, vIndex, vSegment);
        void setRandomServer();
public:
        BlockProxy() : Server() {}
        virtual ~BlockProxy() {}
};

#endif /* _BLOCKPROXY_H */
