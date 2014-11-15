#include "common.h"
#include "blockproxy.h"
#include "video.h"
#include "filesystem.h"
#include "logging.h"
using namespace std;

class Proxy : public BlockProxy
{
private:
        unordered_map<vIndex, size_t> requestedVideos;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage* msg);
public:
        Proxy() : BlockProxy() {}
        virtual ~Proxy() {}
};

Define_Module(Proxy);

void Proxy::initialize()
{
        BlockProxy::initialize();
}

void Proxy::handleMessage(cMessage *msg)
{
        if (!strcmp(msg->getName(), "teardown")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                vSegment s = p->getSeg() + 1;
                while (cache.exists(VideoSegment(i,s))) ++s;
                if (s < requestedVideos[i]) {
                        queryVideoSegment(i, s);
                        logging(LOG_QRY, addr, centralServerIP, i, s);
                } else {
                        requestedVideos.erase(i);
                }
                delete p;
        } else if (!strcmp(msg->getName(), "close")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                if (!FileSystem::instance().exists(addr, i) &&
                    !requestedVideos.count(i)) {
                        vSegment s = p->getSeg() + 1;
                        for (; cache.exists(VideoSegment(i,s)); ++s);
                        size_t m = min(cache.maxSize()/10,
                                       VideoCollection::instance().getVideoSegment(i));
                        if (s < m) {
                                requestedVideos.emplace(i, m);
                                queryVideoSegment(i, s);
                                logging(LOG_QRY, addr, centralServerIP, i, s);
                        }
                }
                delete p;
        } else {
                BlockProxy::handleMessage(msg);
        }
}
