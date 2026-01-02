#include "PlrApp.h"
#include <omnetpp.h>

using namespace omnetpp;
using namespace veins;

Define_Module(veins::PlrApp);

simsignal_t PlrApp::sentPkSig = cComponent::registerSignal("sentPk");
simsignal_t PlrApp::recvPkSig = cComponent::registerSignal("recvPk");

void veins::PlrApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        double f = par("sendFreq").doubleValueInUnit("Hz");
        if (f <= 0) throw cRuntimeError("sendFreq must be > 0");
        sendInterval = 1/f;
        nodeId = getParentModule()->getIndex();
        payloadBytes = par("payloadBytes").intValue();
        sendTimer = new cMessage("sendTimer");
        pairAttempts200 = 0;
        pairSuccess200  = 0;
        // 首次触发 = 现在 + 抖动（非 0）
        simtime_t first = simTime();
        if(simTime().dbl()<23500)
        {
            scheduleAt(23500.5, sendTimer);
        }
        else
        {
            scheduleAt(first+0.15+uniform(0,0.05), sendTimer);
        }

    }
}

void veins::PlrApp::handleSelfMsg(cMessage* msg) {
    if (msg == sendTimer) {
        auto* bm = new BeaconMessage("bm");
        populateWSM(bm);
        bm->setByteLength(payloadBytes);
        bm->setSenderId(nodeId);
        bm->setSenderPos(curPosition);
        bm->setSigVrfd(true);
        bm->setTimestamp(simTime());
        // 可选：若你的 BeaconMessage 有这些字段就设上（没有就忽略）
        // bm->setRecipientAddress(-1);              // 广播
        // bm->setUserPriority(3);                   // VO
        // bm->setChannelNumber(0);                  // CCH
        cModule* mgrMod = getParentModule()->getParentModule()->getSubmodule("manager");
        auto* manager = check_and_cast<TraCIScenarioManager*>(mgrMod);
        const auto& hosts = manager->getManagedHosts();

        auto* myMob = TraCIMobilityAccess().get(getParentModule());
        Coord myPos = myMob->getPositionAt(simTime());

        int cnt = 0;
        for (auto& kv : hosts) {
        cModule* other = kv.second;
        if (other == getParentModule()) continue;                 // 排除自己
        auto* om = check_and_cast<TraCIMobility*>(other->getSubmodule("veinsmobility"));
        if (currentDistTo(om->getPositionAt(simTime())) <= plrRange) cnt++;
        }
        pairAttempts200 += cnt;
        scheduleAt(simTime() + sendInterval+uniform(-0.002,0.002), sendTimer);
        sendDown(bm);                                // ✅ 走 Mac1609_4/PhyLayer80211p
        sentCount++;
        emit(sentPkSig, 1L);

         // 永不在同刻重排
    } else {
        delete msg;
    }
}

void veins::PlrApp::handleMessage(cMessage* msg) {
    // 非自消息交给基类；它会回调 onWSM(...)
    DemoBaseApplLayer::handleMessage(msg);
}

void veins::PlrApp::onWSM(BaseFrame1609_4* wsm) {
    auto* bm = check_and_cast<BeaconMessage*>(wsm);
    simtime_t sending = bm->getTimestamp();
    // 若要排除自收自发，给 BeaconMessage 增/用 senderAddress 字段判断
    Coord sp     = bm->getSenderPos();
    if (bm->getKind()==5) {
            pairSuccess200++;
    }
    recvCount++;
    emit(recvPkSig, 1L);
}

void veins::PlrApp::finish() {
    recordScalar("pkSentCount", (double)sentCount);
    recordScalar("pkRecvCount", (double)recvCount);
    recordScalar("pairAttempts200", (double)pairAttempts200);
    recordScalar("pairSuccess200",  (double)pairSuccess200);
    DemoBaseApplLayer::finish();
}

Coord veins::PlrApp::getCurPosition() {
    return curPosition;
}
double veins::PlrApp::currentDistTo(Coord pos) {
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    return currentDistTo(mobility->getPositionAt(SimTime()), pos.x, pos.y);
}

double veins::PlrApp::currentDistTo(double x, double y) {
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    return currentDistTo(mobility->getPositionAt(SimTime()), x, y);
}

double veins::PlrApp::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::PlrApp::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}
