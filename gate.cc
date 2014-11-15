#include <vector>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include "common.h"
#include "utils.h"
#include "iptools.h"
#include "video.h"
#include "logging.h"
using namespace std;

class Gate : public cSimpleModule
{
private:
        size_t numberOfVideos;
        ipaddr addr;
        cModule *parent;
        vector<vIndex> videos;
        unordered_set<vIndex> hotspot; // Video Hotspot
        vector<ipaddr> cluster;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void replyVideoInfo(ipaddr dst, vIndex i, vSegment s, cValue v);
        ipaddr hashing(vIndex i) const;

public:
        Gate() : cSimpleModule() {}
        virtual ~Gate() {}
};

Define_Module(Gate);

void Gate::initialize()
{
        parent = getParentModule();
        addr = parent->par("address");
        numberOfVideos = par("numberOfVideos");
        srand(parent->getIndex());
        vector<int> v(VideoCollection::instance().size());
        iota(v.begin(), v.end(), 0);
        random_shuffle(v.begin(), v.end());
        videos.assign(v.begin(), v.begin()+numberOfVideos);
        sort(videos.begin(), videos.end());
        cXMLElement *root = par("cluster").xmlValue();
        for (auto child=root->getFirstChild(); child;
             child=child->getNextSibling()) {
                ipaddr v = atoi(child->getNodeValue());
                if (is_server(v))
                        cluster.push_back(v);
        }
}

ipaddr Gate::hashing(vIndex i) const
{
        size_t offset = 0;
        if (hotspot.count(i)) {
                offset = intuniform(0, REPLICA_NUMBER-1);
        }
        return cluster[(i+offset)%cluster.size()];
}

void Gate::handleMessage(cMessage *msg)
{
        if (!strcmp(msg->getName(), "new_video")) {
                Paket *p = check_and_cast<Paket*>(msg);
                vIndex i = fzipf();
                for (; i > videos.size(); i = fzipf());
                i = videos[i-1];
                vSegment s = VideoCollection::instance().getVideoSegment(i);
                cValue serverIP = hashing(i);
                replyVideoInfo(p->getSrc(), i, s, serverIP);
                logging(LOG_NVI, addr, p->getSrc(), i, s, serverIP);
                delete p;
        } else if (!strcmp(msg->getName(), "operation")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                if (p->getV() == VIDEO_LIFT) {
                        hotspot.emplace(i);
                } else if (p->getV() == VIDEO_DROP) {
                        hotspot.erase(i);
                }
                logging(LOG_OPE, addr, p->getSrc(), i, 0, p->getV());
                delete p;
        } else {
                EV << "[ERR] Unexpected message: " << msg->getName() << endl;
                delete msg;
        }
}

void Gate::replyVideoInfo(ipaddr dst, vIndex i, vSegment s, cValue v)
{
        Paket *p = generateVideoInfo(addr, dst, i, s, v);
        send(p, "port$o");
}
