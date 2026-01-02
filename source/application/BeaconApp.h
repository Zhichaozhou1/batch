#ifndef BeaconApp_H
#define BeaconApp_H

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
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
class BeaconApp: public DemoBaseApplLayer{
    protected:
        std::queue<cMessage*> pendingQueue_h;
        std::queue<cMessage*> pendingQueue_m;
        std::queue<cMessage*> pendingQueue_l;
        std::unordered_map<std::string, std::list<omnetpp::cMessage*>::iterator> indexMap;
        std::unordered_map<std::string, std::list<omnetpp::cMessage*>::iterator> movedtoqueuehMap;
        std::unordered_map<std::string, std::list<BeaconMessage*>::iterator> geocastMap;
        std::map<std::pair<int, int>, simtime_t> vehiclediscovery;
        std::map<std::pair<int, int>, simtime_t> vehiclestatus;
        //std::map<std::pair<int, int>, simtime_t> vehiclerecv;
        std::map<std::pair<int, int>, int> vehiclerecvnum;
        std::map<std::pair<int, int>, int> vehiclerecvnum_pre;
        std::map<std::pair<int, int>, int> brakingrecv;
        std::map<std::pair<int, int>, Coord> brakingdistance;
        cMessage* processQueueEvent;
        cMessage* broadcastTimer;
        cMessage* geocastTimer;
        cMessage* broadcastSolutionTimer;
        cMessage* geocastSolutionTimer;
        cMessage* messageVrfd;
        cMessage* braking;
        cMessage* emergencyannounce;
        cMessage* emannounceSolutionTimer;
        cMessage* distance;
        cMessage* geocasttrans;
        std::list<cMessage*> messageList; //waiting for solution
        std::list<cMessage*> messageList2; //moved from ql to qh
        std::list<BeaconMessage*> messageList3; //geocastmsg
        std::list<BeaconMessage*> messageList4; //geocast msg need to be trans
        veins::TraCICommandInterface::Vehicle* veh = nullptr;

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
        bool removeMessagefromdoubleqlByKeyId(const std::string& keyId);
        std::string generateRandomString(size_t length);
        void geocast(BeaconMessage* bm);
        void geocastSolutionMessage();
        void emergencybraking();
        void emannounce();
        void emannounceSolutionMessage();
        void distancecheck();
    protected:
        std::vector<int> cachedpsdy;
        std::string MsgId;
        std::string geoMsgId;
        std::string announceId;
        std::string geocastId;
        int beaconId;
        int flag_queue; // queue_l:0, queue_m:1, queue_h:2
        double interestedRange = 200;
        double puzzlesolution;
        int PC_lifetime = 100;
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
        int sentCount;
        int recvCount;
        double lifeTime;
        bool startScalar = false;
        //double certVrfcDelay = 0.0015;
        //double msgVrfcDelay = 0.0015;
        double certVrfcDelay;// = par("Delay").doubleValue();
        double msgVrfcDelay;// = par("Delay").doubleValue();
        Coord pos_cache;
        int keyId_cache;
        int senderId_cache;
        double braking_time;
        double packet_loss = 1;
        simtime_t braking_at;
        Coord braking_pos;
        int repeat_time = 30;
};
}
#endif
