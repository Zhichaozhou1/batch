#include "veins/modules/application/traci/ADVApp.h"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
using namespace veins;
Define_Module(veins::ADVApp);

void veins::ADVApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        //nodeId = getParentModule()->getIndex();
        nodeId = getParentModule()->getIndex();
        beaconId = 0;
        KeyId = -1;
        lifeTime = par("lifeTime").doubleValue();
        broadcastTimer = new cMessage("broadcastTimer");
        interval = par("Delay").doubleValue();
        msgkind = par("kind").intValue();
        if(simTime().dbl()<23400)
        {
            scheduleAt(23400, broadcastTimer);
        }
        else
        {
            //scheduleAt(simTime() + 0.01 + uniform(-0.002,0.002), broadcastTimer);
            scheduleAt(simTime() + 0.01, broadcastTimer);
        }
        //scheduleAt(simTime() + 0.01, broadcastTimer);
    }
}

void veins::ADVApp::handleMessage(cMessage* msg){

    if (msg->isSelfMessage()) {
            if (msg == broadcastTimer) {
                if(uniform(0,1)<packet_loss)
                {
                    broadcastMessage();
                }
                //scheduleAt(simTime() + interval + uniform(-0.000005,0.000005), broadcastTimer);
                scheduleAt(simTime() + interval, broadcastTimer);
            }
            else {
                delete msg;
            }
        }
    else {

    }
}

void veins::ADVApp::broadcastMessage()
{
    BeaconMessage* bm = new BeaconMessage();
    populateWSM(bm);
    int L= 500;
    bm->setByteLength(L);
    std::string randomStr = generateRandomString(8);
    bm->setTimestamp(simTime());
    bm->setKind(msgkind);
    bm->setSenderId(nodeId);
    bm->setBeaconId(beaconId++);
    bm->setKeyId(KeyId);
    bm->setSigVrfd(false);
    bm->setPCVrfd(false);
    bm->setHashchainVrfd(false);
    bm->setMsgId(randomStr.c_str());
    MsgId = randomStr;
    //int numHosts = getParentModule()->getParentModule()->getSubmodule("vehicle", 0)->getVectorSize();
    //EV << "sending to" << "\n";
/*
    cModule* mgrMod = getParentModule()->getParentModule()->getSubmodule("manager");
    TraCIScenarioManager* manager = dynamic_cast<TraCIScenarioManager*>(mgrMod);
    auto& hosts = manager->getManagedHosts();
    //for (int i = 0; i < numHosts; i++) {
    for(auto it = hosts.begin(); it != hosts.end(); ++it){
        //if (nodeId != i) {
        cModule* tmpNode = it->second;
        if (tmpNode == getParentModule()) continue;
        cModule* mobMod = tmpNode->getSubmodule("veinsmobility");
        TraCIMobility* tmpMob = dynamic_cast<TraCIMobility*>(mobMod);
        Coord tmpPosition = tmpMob->getPositionAt(simTime());
            //cModule *tmpNode = getParentModule()->getParentModule()->getSubmodule("vehicle", i);
            //BeaconApp *tmpApp =dynamic_cast<BeaconApp *>(tmpNode->getSubmodule("appl"));
            //Coord tmpPosition = tmpApp->getCurPosition();
            //if (interestedRange == 0||currentDistTo(tmpPosition)<= interestedRange) {
        sendDirect(bm->dup(), tmpNode->getSubmodule("appl"), "beaconradioIn");
            //}
        //}
    }
*/
    sendDown(bm->dup());
    delete bm;
}


Coord veins::ADVApp::getCurPosition() {
    return curPosition;
}
double veins::ADVApp::currentDistTo(Coord pos) {
    return currentDistTo(curPosition, pos.x, pos.y);
}

double veins::ADVApp::currentDistTo(double x, double y) {
    return currentDistTo(curPosition, x, y);
}

double veins::ADVApp::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::ADVApp::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void veins::ADVApp::finish(){

        DemoBaseApplLayer::finish();
}


std::string veins::ADVApp::generateRandomString(size_t length) {
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


