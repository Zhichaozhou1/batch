#ifndef BatchVeh_H
#define BatchVeh_H

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/messages/BatchVeri_m.h"
#include "veins/base/utils/FindModule.h"
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
class BatchVeh: public DemoBaseApplLayer{
    protected:
        cMessage* broadcastTimer;
        cMessage* broadcastSolutionTimer;
    protected:
        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *msg) override;
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
        std::string generateRandomString(size_t length);
    protected:
        std::vector<int> cachedpsdy;
        std::string MsgId;
        std::string geoMsgId;
        std::string announceId;
        std::string geocastId;
        int beaconId;
        int flag_queue; // queue_l:0, queue_m:1, queue_h:2
        double interestedRange = 300;
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
