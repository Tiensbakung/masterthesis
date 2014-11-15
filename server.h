#ifndef _SERVER_H_
#define _SERVER_H_
#include <unordered_set>
#include <list>
#include "utils.h"
#include "lrucache.h"
#define CACHE_SIZE 1000


struct Connection
{
        ipaddr client;
        vIndex index;
        vSegment seg;
        Connection(ipaddr c=0, vIndex i=0, vSegment s=0) :
                client(c), index(i), seg(s) {
        }
        bool operator<(const Connection& b) const {
                if (client < b.client)
                        return true;
                else if (client > b.client)
                        return false;
                else if (index < b.index)
                        return true;
                else if (index > b.index)
                        return false;
                else
                        return (seg < b.seg);
        }

        bool operator==(const Connection& b) const {
                return (client == b.client &&
                        index == b.index &&
                        seg == b.seg);
        }
};

enum ConnType {
        LOCAL,                  // requested video is in local disk
        CACHED,                 // requested video is in cache
        NEW                     // requested video is not found
};

struct ConnData
{
        ipaddr c;               // Client IP address
        vIndex i;               // Video ID Index
        vSegment s;             // Video Segment Index
        vSequence v;            // Packet sequence number
        ConnType t;             // Connection Type
        ConnData(ipaddr c_, vIndex i_, vSegment s_, vSequence v_, ConnType t_):
                c(c_), i(i_), s(s_), v(v_), t(t_) {
        }
};

namespace std {
        template <>
        struct hash < Connection >
        {
                size_t operator()(const Connection& k) const {
                        return hash<ipaddr>()(k.client) ^
                                hash<vIndex>()(k.index) ^
                                hash<vSegment>()(k.seg);
                }
        };
};


typedef std::list<ConnData> ConnTable;
typedef ConnTable::iterator ConnTableIterator;
typedef std::unordered_map<Connection, ConnTableIterator> ConnHash;
typedef LRUCache<VideoSegment, vSequence> Cache;

class Server: public cSimpleModule
{
private:
        cMessage *handleRequestEvent;
        ConnHash connServ;
        ConnTable connTable;
protected:
        ipaddr addr;
        cModule *parent;
        ipaddr centralServerIP;
        Cache cache;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage*);
        bool exists(vIndex i, vSegment s) const;
        void handleRequests();
        void queryVideoSegment(vIndex i, vSegment s);
        void requestVideoSegement(ipaddr, vIndex, vSegment);
        void scheduleRequestEvent();
        void sendTearDownNotify(ipaddr, vIndex, vSegment);
        void servRequest(ipaddr, vIndex, vSegment, cValue);
        void setTimer(cMessage*, simtime_t);
        void setupConnection(ipaddr, vIndex, vSegment);
        void teardownConnection(ipaddr c, vIndex i, vSegment s);
public:
        Server() : cSimpleModule(), cache(CACHE_SIZE) {
                handleRequestEvent = NULL;
                parent = NULL;
        }
        virtual ~Server() {
                cancelAndDelete(handleRequestEvent);
        }
};

#endif  /* _SERVER_H_ */
