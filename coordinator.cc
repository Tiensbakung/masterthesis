#include <iterator>
#include <algorithm>
#include "common.h"
#include "utils.h"
#include "filesystem.h"
#include "iptools.h"
#include "logging.h"
using namespace std;

typedef ipaddr ClusterID;       // a.k.a Central Server IP
typedef set<vIndex> Hotspot;
typedef vector<ipaddr> IPVec;
typedef vector<pair<vSegment, size_t>> SegmentCounts;
typedef map<VideoSegment, set<VideoSegment>> PrecachingHints;
typedef unordered_map<ipaddr, ClusterID> ServerToClusterMap;
typedef unordered_map<ClusterID, IPVec> ClusterToIPVec;
typedef unordered_map<ClusterID, unordered_map<vIndex, unordered_map<vSegment, size_t>>> StatsData;

class Coordinator : public cSimpleModule
{
private:
        cModule *parent;
        cMessage *precachingAnalysisEvent;
        cMessage *hotspotAnalysisEvent;
        ipaddr addr;
        ClusterToIPVec cts;
        ClusterToIPVec ctg;
        int hotspotAnalysisInterval;
        int hotspotMinHits;
        int hotspotPercent;
        Hotspot hotspot;
        int precachingAnalysisInterval;
        int precachingPercent;
        PrecachingHints precachingHints;
        StatsData statsData;
        StatsData tempStatsData;
        ServerToClusterMap stc;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void aggregateClusterStats(ClusterID cid, const Stats& stats);
        void analyzeHotspot(Hotspot& drops, Hotspot& lifts);
        void anticipatePrecaching();
        void anticipateSingleVideoPrecaching(vIndex i, SegmentCounts& P);
        void distributeVideoOperator(const IPVec& S, vIndex i, cValue v);
        void eliminateDuplicateHints();
        void findDesignatedServerIPs(IPVec& S, vIndex i);
        ipaddr findOriginServerIP(vIndex i, vSegment s);
        void replySegmentQuery(ipaddr, vIndex, vSegment, cValue);
        void sendSegmentPrecachingHint(ipaddr, vIndex, vSegment);
        void scheduleHotspotAnalysisEvent();
        void schedulePrecachingAnalysisEvent();
        void sendDrops(const Hotspot& drops);
        void sendLifts(const Hotspot& lifts);
        void setTimer(cMessage* msg, simtime_t delta);
        void udpateHotspot(const Hotspot& drops, const Hotspot& lifts);
public:
        Coordinator() : cSimpleModule(), parent(NULL),
                        precachingAnalysisEvent(NULL),
                        hotspotAnalysisEvent(NULL) {}
        virtual ~Coordinator() {
                cancelAndDelete(precachingAnalysisEvent);
                cancelAndDelete(hotspotAnalysisEvent);
        }
};

Define_Module(Coordinator);

void Coordinator::initialize()
{
        cXMLElement *root = par("clusters").xmlValue();
        for (auto l1=root->getFirstChild(); l1; l1=l1->getNextSibling()) {
                ClusterID cid = atoi(l1->getAttribute("id"));
                for (auto l2 = l1->getFirstChild(); l2; l2=l2->getNextSibling()) {
                        ipaddr v = atoi(l2->getNodeValue());
                        if (is_server(v)) {
                                stc[v] = cid;
                                cts[cid].push_back(v);
                        } else if (is_gate(v)) {
                                ctg[cid].push_back(v);
                        }
                }
        }

        parent = getParentModule();
        addr = parent->par("address");
        hotspotAnalysisInterval = par("hotspotAnalysisInterval");
        hotspotPercent = par("hotspotPercent");
        hotspotMinHits = par("hotspotMinHits");
        precachingAnalysisInterval = par("precachingAnalysisInterval");
        precachingPercent = par("precachingPercent");
        if (par("analysisStats").boolValue() == true) {
                hotspotAnalysisEvent = new cMessage("hotspotAnalysisEvent");
                scheduleHotspotAnalysisEvent();
                precachingAnalysisEvent = new cMessage("precachingAnalysisEvent");
                schedulePrecachingAnalysisEvent();
        }
}

void Coordinator::handleMessage(cMessage *msg)
{
        if (msg == precachingAnalysisEvent) {
                anticipatePrecaching();
                // cout << "Stats:" <<endl;
                // for (auto& t: statsData) {
                //         cout << "\n** Cluster " << t.first << ":" << endl;
                //         for (auto &i: t.second) {
                //                 cout << i.first << ": " ;
                //                 for (auto &s: i.second) {
                //                     cout << "   " << s.first << "@" << s.second ;
                //                 }
                //                 cout << endl;
                //         }
                // }
                // cout << "Precaching hints:" <<endl;
                // for (auto& t: precachingHints) {
                //         cout << "## (" << t.first.first
                //            << "," << t.first.second << ") {";
                //         for (auto& t2: t.second) {
                //                 cout << "(" << t2.first
                //                    << "," << t2.second << ") ";
                //         }
                //         cout << "}" << endl;
                // }
                schedulePrecachingAnalysisEvent();
        } else if (msg == hotspotAnalysisEvent) {
                Hotspot lifts, drops;
                analyzeHotspot(drops, lifts);
                udpateHotspot(drops, lifts);
                EV << "New hotspots: {";
                for (auto& x: hotspot) { EV << x << " "; }
                EV << "}" << endl;
                sendDrops(drops);
                sendLifts(lifts);
                tempStatsData.clear();
                scheduleHotspotAnalysisEvent();
        } else if (!strcmp(msg->getName(), "query")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                vSegment s = p->getSeg();
                ipaddr originServerIP = findOriginServerIP(i, s);
                replySegmentQuery(p->getSrc(), i, s, originServerIP);
                logging(LOG_REP, addr, p->getSrc(), i, s, originServerIP);
                sendSegmentPrecachingHint(p->getSrc(), i, s);
                delete p;
        } else if (!strcmp(msg->getName(), "segment_hint")) {
                Paket *p = check_and_cast<Paket *>(msg);
                vIndex i = p->getIndex();
                vSegment s = p->getSeg();
                sendSegmentPrecachingHint(p->getSrc(), i, s);
                delete p;
        } else if (!strcmp(msg->getName(), "report")) {
                Report *p = check_and_cast<Report *>(msg);
                ClusterID clusterID = stc[p->getSrc()];
                aggregateClusterStats(clusterID, p->getStats());
                // EV << "Aggregate stats for cluster "<< clusterID << endl;
                // for (auto& t: statsData[clusterID]) {
                //         EV << "##" << t.first << " {";
                //         for (auto& t2: t.second) {
                //                 EV << "(" << t2.first
                //                    << "," << t2.second << ") ";
                //         }
                //         EV << "}" << endl;
                // }
                delete p;
        } else {
                EV << "[ERR] Unexpected message:" << msg->getName() << endl;
                delete msg;
        }
}

void Coordinator::findDesignatedServerIPs(IPVec& S, vIndex i)
{
        int n = 1;
        if (hotspot.count(i)) {
                n = REPLICA_NUMBER;
        }
        for (int j = 0; j < n; ++j) {
                size_t offset = (i+j) % cts[addr].size();
                S.push_back(cts[addr][offset]);
        }
}

ipaddr Coordinator::findOriginServerIP(vIndex i, vSegment s)
{
        ServerSet ss = FileSystem::instance().servers(i);
        vector<ipaddr> v;
        for (auto& t: ss) {
                if (stc[t] == stc[addr]) {
                        v.push_back(t);
                }
        }
        if (v.size()) {         // Video found in same cluster
                return v[intuniform(0, v.size()-1)];
        } else {                // Video found in other clusters
                size_t offset = intuniform(0, ss.size()-1);
                auto it = ss.begin();
                advance(it, offset);
                return *it;
        }
}

void Coordinator::analyzeHotspot(Hotspot& drops, Hotspot& lifts)
{
        vector<pair<size_t, vIndex>> hits;
        size_t n = 0;
        for (auto& t: tempStatsData[addr]) {
                ++n;
                size_t count = 0;
                if (!t.second.empty()) {
                        count = t.second[0];
                }
                // cout << t.first << ": " << count << endl;
                //for (auto& t2: t.second) { count += t2.second; }
                if (count >= hotspotMinHits)
                        hits.push_back(pair<size_t, vIndex>(count,t.first));
        }
        n = n * hotspotPercent / 100;
        // cout << "N is " << n << endl;
        Hotspot hs;
        sort(hits.begin(), hits.end());
        size_t i=0;
        for (auto it = hits.begin(); it != hits.end() && i<n; ++i, ++it) {
                hs.emplace(it->second);
        }
        set_difference(hotspot.begin(), hotspot.end(), hs.begin(), hs.end(),
                       inserter(drops, drops.end()));
        set_difference(hs.begin(), hs.end(), hotspot.begin(), hotspot.end(),
                       inserter(lifts, lifts.end()));
}

void Coordinator::udpateHotspot(const Hotspot& drops, const Hotspot& lifts)
{
        for (auto x: drops) { hotspot.erase(x); }
        hotspot.insert(lifts.begin(), lifts.end());
}

void Coordinator::distributeVideoOperator(const IPVec& S, vIndex i, cValue v) {
        for (auto& dst: S) {
                Paket *p = generateVideoOperation(addr, dst, i, 0, v);
                send(p, "port$o");
        }
}

void Coordinator::sendDrops(const Hotspot& drops)
{
        for (auto& i: drops) {
                IPVec S;
                findDesignatedServerIPs(S, i);
                S.erase(S.begin());
                distributeVideoOperator(S, i, VIDEO_DROP);
                distributeVideoOperator(ctg[addr], i, VIDEO_DROP);
        }
}

void Coordinator::sendLifts(const Hotspot& lifts)
{
        for (auto& i: lifts) {
                IPVec S;
                findDesignatedServerIPs(S, i);
                distributeVideoOperator(S, i, VIDEO_LIFT);
                distributeVideoOperator(ctg[addr], i, VIDEO_LIFT);
        }
}

void Coordinator::aggregateClusterStats(ClusterID cid, const Stats& stats)
{
        for (auto& t: stats) {
                vIndex i = t.first.first;
                vSegment s = t.first.second;
                statsData[cid][i][s] += t.second;
                tempStatsData[cid][i][s] += t.second;
        }
}

void Coordinator::anticipateSingleVideoPrecaching(vIndex i, SegmentCounts& P)
{
        for (auto j = P.begin(); j != P.end(); ++j) {
                const size_t c_j = j->second;
                for (auto k = P.begin(); k != j; ++k) {
                        const size_t c_k = k->second;
                        VideoSegment s_k(i, k->first);
                        if (c_k * precachingPercent <= 100 * c_j) {
                                precachingHints[s_k].emplace(i, j->first);
                        } else if (precachingHints.count(s_k)) {
                                VideoSegment s_j(i, j->first);
                                precachingHints[s_k].erase(s_j);
                        }
                }
        }
}

void Coordinator::eliminateDuplicateHints()
{
        for (auto& t: precachingHints) {
                set<VideoSegment> S;
                for (auto k = t.second.begin(); k != t.second.end(); ++k) {
                        for (auto l = t.second.begin(); l != k; ++l) {
                                if (precachingHints.count(*l) &&
                                    precachingHints[*l].count(*k)) {
                                        S.insert(*k);
                                        break;
                                }
                        }
                }
                for (auto& s: S) { t.second.erase(s); }
                if (t.second.size() > 10) {
                        auto it = t.second.begin();
                        advance(it, 10);
                        t.second.erase(it, t.second.end());
                }
        }
}

void Coordinator::anticipatePrecaching()
{
        for (auto& t: statsData[addr]) {
                vIndex i = t.first;
                SegmentCounts P;
                for (auto& t2: t.second) {
                        if (t2.second >= 5) {
                                P.push_back(t2);
                        }
                }
                sort(P.begin(), P.end());
                anticipateSingleVideoPrecaching(i, P);
        }
        eliminateDuplicateHints();
}

void Coordinator::replySegmentQuery(ipaddr dst, vIndex i, vSegment s, cValue v)
{
        Paket *reply = generateReply(addr, dst, i, s, v);
        send(reply, "port$o");
}

void Coordinator::sendSegmentPrecachingHint(ipaddr dst, vIndex i, vSegment s)
{
        VideoSegment e(i, s);
        if (precachingHints.count(e) == 0) return;
        for (auto& t: precachingHints[e]) {
                vIndex i2 = t.first;
                vSegment s2 = t.second;
                Paket *hint = generatePrecachingHint(addr, dst, i2, s2);
                send(hint, "port$o");
        }
}

void Coordinator::scheduleHotspotAnalysisEvent()
{
        setTimer(hotspotAnalysisEvent, hotspotAnalysisInterval);
}

void Coordinator::schedulePrecachingAnalysisEvent()
{
        setTimer(precachingAnalysisEvent, precachingAnalysisInterval);
}

void Coordinator::setTimer(cMessage* msg, simtime_t delta)
{
        scheduleAt(simTime()+delta, msg);
}
