#ifndef BeaconAppTESLA_H
#define BeaconAppTESLA_H

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"

#include "veins/modules/messages/BeaconMessage_m.h"
#include <string>
#include <list>
#include <math.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <random>
namespace veins {
class BeaconAppTESLA: public DemoBaseApplLayer{
    protected:
        std::queue<cMessage*> pendingQueue_h;
        std::queue<cMessage*> pendingQueue_m;
        std::queue<cMessage*> pendingQueue_l;
        std::unordered_map<std::string, std::list<omnetpp::cMessage*>::iterator> indexMap;
        std::map<std::pair<int, int>, simtime_t> vehiclediscovery;
        cMessage* processQueueEvent;
        cMessage* broadcastTimer;
        cMessage* broadcastSolutionTimer;
        cMessage* messageVrfd;
        std::list<cMessage*> messageList; //waiting for solution
    protected:
        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void processPendingQueue();
        virtual void onWSM(BaseFrame1609_4* frame) override;
        virtual void finish() override;
        void broadcastMessage();
        void broadcastSolutionMessage();
        Coord getCurPosition();
        double currentDistTo(double x, double y);
        double currentDistTo(Coord pos);
        double currentDistTo(Coord pos1, Coord pos2);
        double currentDistTo(Coord pos, double x, double y);
        double currentDistTo(double x1, double y1, double x2, double y2);
        cMessage* findMessageByKeyId(const std::string& keyId);
        bool removeMessageByKeyId(const std::string& keyId);
        std::string generateRandomString(size_t length);
    protected:
        std::vector<int> cachedpsdy;
        std::string MsgId;
        int beaconId;
        int flag_queue; // queue_l:0, queue_m:1, queue_h:2
        double interestedRange = 200;
        double puzzlesolution = 0.01;
        int PC_lifetime = 600;
        int KeyId = 0;
        int beaconradioIn;
        int nodeId;
        int queue_l;
        int queue_m;
        int queue_h;
        int expired_msg;
        int msg_recv;
        int bogus_msg_check;
        int bogus_msg_recv;
        double lifeTime;
        bool startScalar = false;
        double certVrfcDelay = 0.0015;
        double msgVrfcDelay = 0.0015;
};
}
#endif
