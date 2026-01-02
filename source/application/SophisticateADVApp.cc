#include "veins/modules/application/traci/SophisticateADVApp.h"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
using namespace veins;
Define_Module(veins::SophisticateADVApp);

void veins::SophisticateADVApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        //nodeId = getParentModule()->getIndex();
        nodeId = getParentModule()->getId();
        beaconId = 0;
        KeyId = -1;
        lifeTime = par("lifeTime").doubleValue();
        broadcastTimer = new cMessage("broadcastTimer");
        broadcastSolutionTimer = new cMessage("broadcastSolutionTimer");
        scheduleAt(23400, broadcastTimer);
    }
}

void veins::SophisticateADVApp::handleMessage(cMessage* msg){

    if (msg->isSelfMessage()) {
            if (msg == broadcastTimer) {
                broadcastMessage();
                scheduleAt(simTime() + 0.000054, broadcastSolutionTimer);
                scheduleAt(simTime() + 0.00054, broadcastTimer);
            }
            else if (msg == broadcastSolutionTimer)
            {
                broadcastSolutionMessage();
            }
            else {
                delete msg;
            }
        }
    else {

    }
}

void veins::SophisticateADVApp::broadcastMessage()
{
    BeaconMessage* bm = new BeaconMessage();
    int L=par("payloadBytes").doubleValueInUnit("B");
    bm->setByteLength(L);
    std::string randomStr = generateRandomString(16);
    bm->setTimestamp(simTime());
    bm->setKind(0);
    bm->setSenderId(-nodeId);
    bm->setBeaconId(beaconId++);
    bm->setKeyId(KeyId);
    bm->setSigVrfd(false);
    bm->setPCVrfd(false);
    bm->setHashchainVrfd(false);
    bm->setMsgId(randomStr.c_str());
    MsgId = randomStr;
    //int numHosts = getParentModule()->getParentModule()->getSubmodule("vehicle", 0)->getVectorSize();
    //EV << "sending to" << "\n";
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
    delete bm;
}


void veins::SophisticateADVApp::broadcastSolutionMessage()
{
    if (!startScalar) {
        recordScalar("##startTime", simTime().dbl());
        recordScalar("##startX", curPosition.x);
        recordScalar("##startY", curPosition.y);
        startScalar = true;
    }
    BeaconMessage* bm = new BeaconMessage();
    int L=par("solpayloadBytes").doubleValueInUnit("B");
    bm->setByteLength(L);
    bm->setTimestamp(simTime());
    bm->setKind(1);
    bm->setSenderId(nodeId);
    bm->setPuzzleVrfd(true);
    bm->setMsgId(MsgId.c_str());
    //int numHosts = getParentModule()->getParentModule()->getSubmodule("vehicle", 0)->getVectorSize();
    //EV << "sending to" << "\n";
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
        sendDirect(bm->dup(), tmpNode->getSubmodule("appl"), "beaconradioIn");
        //}
    }
    delete bm;
}



Coord veins::SophisticateADVApp::getCurPosition() {
    return curPosition;
}
double veins::SophisticateADVApp::currentDistTo(Coord pos) {
    return currentDistTo(curPosition, pos.x, pos.y);
}

double veins::SophisticateADVApp::currentDistTo(double x, double y) {
    return currentDistTo(curPosition, x, y);
}

double veins::SophisticateADVApp::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::SophisticateADVApp::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void veins::SophisticateADVApp::finish(){

        DemoBaseApplLayer::finish();
}


std::string veins::SophisticateADVApp::generateRandomString(size_t length) {
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


