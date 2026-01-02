#include "veins/modules/application/traci/ADVBatch.h"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
using namespace veins;
Define_Module(veins::ADVBatch);
void veins::ADVBatch::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        nodeId = getParentModule()->getId();
        nodeId = -nodeId;
        beaconId = 0;
        KeyId = 0;
        broadcastTimer = new cMessage("broadcastTimer");
        broadcastSolutionTimer = new cMessage("broadcastSolutionTimer");
        interval = par("Delay").doubleValue();
        puzzlesolution = par("puzzle").doubleValue();
        if(simTime().dbl()<23400)
        {
            scheduleAt(23400.5, broadcastTimer);
        }
        else
        {
            scheduleAt(simTime(), broadcastTimer);
        }
        beaconradioIn = findGate("beaconradioIn");
    }
}

void veins::ADVBatch::handleMessage(cMessage* msg){
    if (msg->isSelfMessage()) {
            if (msg == broadcastTimer) {
                broadcastMessage();
                EV_ERROR << "DEBUG: broadcastMessage from " << getFullPath() << endl;
                scheduleAt(simTime() + interval, broadcastTimer);
            }
            else if (msg == broadcastSolutionTimer)
            {
                broadcastSolutionMessage();
            }
            else {
                delete msg;
            }
    }
    else
    {
        delete msg;
    }
}

void veins::ADVBatch::broadcastMessage()
{
    BatchVeri* bm = new BatchVeri();
    bm->setTimestamp(simTime());
    bm->setKind(0);
    bm->setSenderId(nodeId);
    bm->setBeaconId(beaconId);
    bm->setKeyId(KeyId);
    bm->setSigVrfd(false);
    bm->setPCVrfd(false);
    bm->setHashchainVrfd(false);
    std::string randomStr = generateRandomString(17);
    bm->setMsgId(randomStr.c_str());
    MsgId = randomStr;
    for (int i = 0; ; ++i) {
            cModule* rsu = getSystemModule()->getSubmodule("rsu", i);
            if (!rsu) break;
            cModule* rsuApp = rsu->getSubmodule("appl");
            if (!rsuApp) continue;
            sendDirect(bm->dup(), rsuApp, "beaconradioIn");
    }

    //sendDown(bm->dup());
    delete bm;
    //recordScalar("##neighborsize", neighbor);
}

void veins::ADVBatch::broadcastSolutionMessage()
{
    if (!startScalar) {
        //recordScalar("##startTime", simTime().dbl());
        //recordScalar("##startX", curPosition.x);
        //recordScalar("##startY", curPosition.y);
        startScalar = true;
    }
    BatchVeri* bm = new BatchVeri();
    populateWSM(bm);
    bm->setTimestamp(simTime());
    bm->setKind(2);
    bm->setSenderId(nodeId);
    bm->setPuzzleVrfd(true);
    bm->setMsgId(MsgId.c_str());
    //int numHosts = getParentModule()->getParentModule()->getSubmodule("vehicle", 0)->getVectorSize();
    //EV << "sending to" << "\n";

    cModule* mgrMod = getParentModule()->getParentModule()->getSubmodule("manager");
    TraCIScenarioManager* manager = dynamic_cast<TraCIScenarioManager*>(mgrMod);
    auto& hosts = manager->getManagedHosts();
    for (int i = 0; ; ++i) {
        cModule* rsu = getSystemModule()->getSubmodule("rsu", i);
        if (!rsu) break;
        cModule* rsuApp = rsu->getSubmodule("appl");
        if (!rsuApp) continue;
        auto* mobMod = rsu->getSubmodule("mobility"); // 可能拿不到
        auto* mm = dynamic_cast<BaseMobility*>(mobMod);
        Coord rsuPos = mm->getPositionAt(simTime());
        //if(interestedRange == 0||currentDistTo(rsuPos)<= interestedRange)
        //{
        sendDirect(bm->dup(), rsuApp, "beaconradioIn");
        //}
    }
    delete bm;
}


void ADVBatch::onWSM(BaseFrame1609_4* frame) {
    // 若你需要处理 WSM，就转型；不需要也至少留个空实现防止链接错误

    // 可选：否则交给基类
    // DemoBaseApplLayer::onWSM(frame);
}

Coord veins::ADVBatch::getCurPosition() {
    return curPosition;
}
double veins::ADVBatch::currentDistTo(Coord pos) {
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    return currentDistTo(mobility->getPositionAt(simTime()), pos.x, pos.y);
}

double veins::ADVBatch::currentDistTo(double x, double y) {
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    return currentDistTo(mobility->getPositionAt(simTime()), x, y);
}

double veins::ADVBatch::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::ADVBatch::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

std::string veins::ADVBatch::generateRandomString(size_t length) {
    static const std::string chars =
        "abcdefghijklmnopqrstuvwxyz";

    static std::mt19937 rng(static_cast<unsigned long>(std::time(nullptr)));
    static std::uniform_int_distribution<> dist(0, static_cast<int>(chars.size() - 1));

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; i++) {
        result.push_back(chars[dist(rng)]);
    }
    return result;
}
void veins::ADVBatch::finish(){
        cancelAndDelete(broadcastTimer);
        DemoBaseApplLayer::finish();
}

