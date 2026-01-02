#pragma once
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/messages/BeaconMessage_m.h"
#include "veins/modules/utility/Consts80211p.h"
namespace veins {
class PlrApp : public veins::DemoBaseApplLayer {
  protected:
    double sendInterval;
    int payloadBytes = 500;
    cMessage* sendTimer = nullptr;
    long sentCount = 0;
    long recvCount = 0;
    int nodeId;
    // 信号
    static omnetpp::simsignal_t sentPkSig;
    static omnetpp::simsignal_t recvPkSig;
    int pairAttempts200;
    int pairSuccess200;
    double plrRange = 200;
  protected:
    virtual void initialize(int stage) override;
    virtual void handleSelfMsg(omnetpp::cMessage* msg) override;
    virtual void handleMessage(omnetpp::cMessage* msg) override; // 交给基类
    virtual void onWSM(veins::BaseFrame1609_4* wsm) override;    // 收到802.11p帧
    virtual void finish() override;
    Coord getCurPosition();
    double currentDistTo(double x, double y);
    double currentDistTo(Coord pos);
    double currentDistTo(Coord pos1, Coord pos2);
    double currentDistTo(Coord pos, double x, double y);
    double currentDistTo(double x1, double y1, double x2, double y2);
};

}
