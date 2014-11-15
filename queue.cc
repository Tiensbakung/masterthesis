#include "paket_m.h"
#include "logging.h"
using namespace std;

class Queue: public cSimpleModule
{
private:
        // int addr;
        cChannel *channel;
        cMessage *selfMsg;
        cQueue queue;
protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void scheduleSelfMsg();
public:
        Queue() : cSimpleModule(), queue() {
                channel = NULL;
                selfMsg = NULL;
        }
        virtual ~Queue() {
                cancelAndDelete(selfMsg);
        }
};

Define_Module(Queue);


void Queue::initialize()
{
        // addr = getParentModule()->par("address");
        channel = gate("egress$o")->getTransmissionChannel();
        selfMsg = new cMessage("selfmsg");
}

void Queue::handleMessage(cMessage *msg)
{
        if (msg->isSelfMessage()) {
                cMessage *ptr = check_and_cast<cMessage*>(queue.pop());
                send(ptr, "egress$o");
                if (!queue.empty()) {
                        scheduleSelfMsg();
                }
                // logging(LOG_QLN, addr, getIndex(), queue.length());
        } else if (channel->isBusy() || queue.length()) {
                queue.insert(msg);
                if (!selfMsg->isScheduled())
                        scheduleSelfMsg();
                // logging(LOG_QLN, addr, getIndex(), queue.length());
        } else {
                send(msg, "egress$o");
        }
}

void Queue::scheduleSelfMsg()
{
        simtime_t t = channel->getTransmissionFinishTime();
        scheduleAt(t, selfMsg);
}
