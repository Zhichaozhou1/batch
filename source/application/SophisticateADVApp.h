#ifndef SophisticateADVApp_H
#define SophisticateADVApp_H

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
class SophisticateADVApp: public DemoBaseApplLayer{

    protected:
        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *msg) override;
        //virtual void onWSM(BaseFrame1609_4* frame) override;
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
        cMessage* broadcastTimer;
        cMessage* broadcastSolutionTimer;
        int beaconId;
        std::string MsgId;
        double interestedRange = 200;
        int PC_lifetime = 600;
        int KeyId = 0;
        int beaconradioIn;
        int nodeId;
        double lifeTime;
        bool startScalar = false;
};
}
#endif
