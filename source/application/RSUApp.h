#ifndef RSUApp_H
#define RSUApp_H

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"

#include "veins/modules/messages/BatchVeri_m.h"
#include "veins/modules/application/traci/KeyList.h"
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
class RSUApp: public DemoBaseApplLayer{

    protected:
        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *msg) override;
        //virtual void onWSM(BaseFrame1609_4* frame) override;
        virtual void finish() override;
        void broadcastMessage();
        void discover();
        Coord getCurPosition();
        double currentDistTo(double x, double y);
        double currentDistTo(Coord pos);
        double currentDistTo(Coord pos1, Coord pos2);
        double currentDistTo(Coord pos, double x, double y);
        double currentDistTo(double x1, double y1, double x2, double y2);
        cMessage* findMessageByKeyId(const std::string& keyId);
        bool removeMessageByKeyId(const std::string& keyId);
        std::string generateRandomString(size_t length);
        virtual void processPendingQueue();
        bool removeMessagefromdoubleqlByKeyId(const std::string& keyId);
    protected:
        cMessage* broadcastTimer;
        cMessage* processQueueEvent;
        cMessage* neighbordiscovery;
        int beaconId;
        double interestedRange = 200;
        int PC_lifetime = 600;
        int KeyId = 0;
        int beaconradioIn;
        int nodeId;
        double batchinterval;
        double packet_loss = 1;
        double lifeTime;
        bool startScalar = false;
        int nodeId_dis;
        int keyId_dis;
        int expired_msg=0;
        KeyList keys;
        std::queue<cMessage*> pendingQueue_h;
        std::queue<cMessage*> pendingQueue_m;
        std::queue<cMessage*> pendingQueue_l;
        std::map<std::pair<int, int>, simtime_t> vehiclediscovery;
        std::unordered_map<std::string, std::list<omnetpp::cMessage*>::iterator> indexMap;
        std::unordered_map<std::string, std::list<omnetpp::cMessage*>::iterator> movedtoqueuehMap;
        std::list<cMessage*> messageList; //waiting for solution
        std::list<cMessage*> messageList2; //moved from ql to qh
        std::map<int,int> cachedpsdy;
        double certVrfcDelay;// = par("Delay").doubleValue();
        double msgVrfcDelay;
        double batch_delay;
        double dis_delay=0;
        int Dis_from_RSU=0;
        int Dis_from_Qh=0;
        int Dis_from_Ql=0;
};
}
#endif
