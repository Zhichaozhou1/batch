#include "veins/modules/application/traci/BeaconAppTESLA.h"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
using namespace veins;
Define_Module(veins::BeaconAppTESLA);

void veins::BeaconAppTESLA::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        cachedpsdy = std::vector<int>(500, -1);
        //nodeId = getParentModule()->getIndex();
        cModule* mobilityModule = getParentModule()->getSubmodule("veinsmobility");
        TraCIMobility* mobility = dynamic_cast<TraCIMobility*>(mobilityModule);
        std::string externalId = mobility->getExternalId();
        std::string numericPart = externalId.substr(3);
        nodeId = std::stoi(numericPart);
        beaconId = 0;
        queue_l = 0;
        queue_m = 0;
        queue_h = 0;
        msg_recv = 0;
        bogus_msg_recv = 0;
        flag_queue = 0;
        expired_msg = 0;
        bogus_msg_check = 0;
        beaconradioIn = findGate("beaconradioIn");
        lifeTime = par("lifeTime").doubleValue();
        processQueueEvent = new cMessage("processQueueEvent");
        broadcastTimer = new cMessage("broadcastTimer");
        broadcastSolutionTimer = new cMessage("broadcastSolutionTimer");
        messageVrfd =  new cMessage("messageVrfd");
        scheduleAt(simTime() + 0.1, broadcastTimer);
        beaconradioIn = findGate("beaconradioIn");
    }
}

void veins::BeaconAppTESLA::handleMessage(cMessage* msg){

    if (msg->isSelfMessage()) {
            if (msg == processQueueEvent) {
                processPendingQueue();
            }
            else if (msg == broadcastTimer) {
                broadcastMessage();
                //EV<<beaconId<<"\n";
                if(beaconId%5 == 0)
                {
                    scheduleAt(simTime() + puzzlesolution, broadcastSolutionTimer);
                }
                scheduleAt(simTime() + 0.1, broadcastTimer);
            }
            else if (msg == broadcastSolutionTimer)
            {
                broadcastSolutionMessage();
            }
            else if (msg == messageVrfd)
            {
                if (!pendingQueue_h.empty()||!pendingQueue_m.empty()||!pendingQueue_l.empty())
                {
                    scheduleAt(simTime(), processQueueEvent);
                }
            }
            //else {
            //    delete msg;
            //}
        }
    else {
            BeaconMessage* bm = check_and_cast<BeaconMessage*>(msg);

            if(bm->getKind()==0){
                if(bm->getSigVrfd())
                {
                    msg_recv++;
                }
                else
                {
                    bogus_msg_recv++;
                }
                if(cachedpsdy[bm->getSenderId()]==bm->getKeyId()&&bm->getHashchainVrfd())
                {
                    pendingQueue_m.push(msg);
                }
                else{
                    int senderId = bm->getSenderId();
                    int keyId = bm->getKeyId();
                    //recordScalar("##message from", senderId);
                    //recordScalar("##keyId", KeyId);
                    //recordScalar("##receive", simTime().dbl());
                    simtime_t ts = bm->getTimestamp();
                    std::pair<int, int> keyPair(senderId, keyId);
                    if (vehiclediscovery.find(keyPair) == vehiclediscovery.end()) {
                        vehiclediscovery[keyPair] = ts;
                    }
                    pendingQueue_l.push(msg);
                    messageList.push_back(msg->dup());
                    auto iter = --messageList.end();
                    std::string key = bm->getMsgId();
                    indexMap[key] = iter;
                }
                if (!messageVrfd->isScheduled()&&!processQueueEvent->isScheduled()) {
                    scheduleAt(simTime(), messageVrfd);
                }
            }
            else if(bm->getKind()==1)
            {
                cMessage* foundMsg = findMessageByKeyId(bm->getMsgId());
                //cMessage* foundMsg = foundmsg.get();
                if(!foundMsg)
                {
                    //EV<<"no such message"<<"\n";
                    return;
                }
                else{
                    //EV<<"find such message"<<"\n";
                    pendingQueue_h.push(foundMsg);
                    bool removed = removeMessageByKeyId(bm->getMsgId());
                    //EV<<"removed message"<<"\n";

                    //EV<<"queued message"<<"\n";
                }
            }
    }
}

void veins::BeaconAppTESLA::processPendingQueue() {
        if (!pendingQueue_h.empty()) {
            cMessage* msg = pendingQueue_h.front();
            pendingQueue_h.pop();
            BaseFrame1609_4* wsm = dynamic_cast<BaseFrame1609_4*>(msg);
            BeaconMessage* bm = check_and_cast<BeaconMessage*>(wsm);
            if (wsm) {
                flag_queue = 2;
                //queue_h++;
                //recordScalar("##Go to Queue_h", 1);
                //recordScalar("##Queue_h length", pendingQueue_h.size());
                //recordScalar("##Queue_m length", pendingQueue_m.size());
                //recordScalar("##Queue_l length", pendingQueue_l.size());
                bool removed = removeMessageByKeyId(bm->getMsgId());
                onWSM(wsm);
            }
            else {
                delete msg;
            }
        }
        else if (!pendingQueue_m.empty())
        {
            cMessage* msg = pendingQueue_m.front();
            pendingQueue_m.pop();
            BaseFrame1609_4* wsm = dynamic_cast<BaseFrame1609_4*>(msg);
            if (wsm) {
                flag_queue = 1;
                //queue_m++;
                //recordScalar("##Go to Queue_m", 1);
                //recordScalar("##Queue_h length", pendingQueue_h.size());
                //recordScalar("##Queue_m length", pendingQueue_m.size());
                //recordScalar("##Queue_l length", pendingQueue_l.size());
                onWSM(wsm);
            }
            else {
                delete msg;
            }
        }
        else if(!pendingQueue_l.empty())
        {
            cMessage* msg = pendingQueue_l.front();
            pendingQueue_l.pop();
            int num = nodeId;
            BaseFrame1609_4* wsm = dynamic_cast<BaseFrame1609_4*>(msg);
            BeaconMessage* bm = dynamic_cast<BeaconMessage*>(wsm);
            bool removed = removeMessageByKeyId(bm->getMsgId());
            if (wsm) {
                flag_queue = 0;
                //queue_l++;
                //recordScalar("##Go to Queue_l", 1);
                //recordScalar("##Queue_h length", pendingQueue_h.size());
                //recordScalar("##Queue_m length", pendingQueue_m.size());
                //recordScalar("##Queue_l length", pendingQueue_l.size());
                onWSM(wsm);
            }
            else {
                delete msg;
            }
        }
        /*if (!pendingQueue_h.empty()||!pendingQueue_m.empty()||!pendingQueue_l.empty())
        {
            scheduleAt(simTime(), processQueueEvent);
        }*/
}

void veins::BeaconAppTESLA::onWSM(BaseFrame1609_4* frame){
    BeaconMessage* bm = check_and_cast<BeaconMessage*>(frame);
    simtime_t now = simTime();
    double delay = 0;
    double waited = 0;
    double discover_waited = 0;
    //bool removed = removeMessageByKeyId(bm->getMsgId());
    if ((now - bm->getTimestamp()).dbl() > lifeTime)
    {
        if(!bm->getSigVrfd())
        {
            bogus_msg_recv--;
        }
        else
        {
            expired_msg++;
        }
        if (!messageVrfd->isScheduled()) {
            scheduleAt(simTime()+delay, messageVrfd);
        }
        return;
    }
    if(cachedpsdy[bm->getSenderId()]!=bm->getKeyId()&&(cachedpsdy[bm->getSenderId()]-1)!=bm->getKeyId()){
        if(bm->getSigVrfd()&&bm->getPCVrfd()){
            //EV<<"PC caching"<<"\n";
            delay = certVrfcDelay + msgVrfcDelay;
            cachedpsdy[bm->getSenderId()]=bm->getKeyId();
            int senderId = bm->getSenderId();
            int keyId = bm->getKeyId();
            std::pair<int, int> keyPair(senderId, keyId);
            auto it = vehiclediscovery.find(keyPair);
            simtime_t storedTimestamp = it->second;
            discover_waited = simTime().dbl()-storedTimestamp.dbl()+delay;
            waited = simTime().dbl()-bm->getTimestamp().dbl()+delay;
            recordScalar("##signerId", bm->getSenderId());
            recordScalar("##KeyId", bm->getKeyId());
            recordScalar("##beaconId", bm->getBeaconId());
            recordScalar("##waitingTime", waited);
            recordScalar("##discoverwaitingTime", discover_waited);
            if(flag_queue == 0)
            {
                queue_l++;
            }
            else if(flag_queue == 1)
            {
                queue_m++;
            }
            else if(flag_queue == 2)
            {
                queue_h++;
            }
        }
        else{
            delay = certVrfcDelay;
            bogus_msg_recv--;
            bogus_msg_check++;
            //EV<<"Adversary beacon"<<"\n";
            //return;
        }
    }
    else if(bm->getSigVrfd())
    {
        if(flag_queue == 0)
        {

        }
        else
        {
            delay = msgVrfcDelay;
            waited = simTime().dbl()-bm->getTimestamp().dbl()+delay;
            recordScalar("##signerId", bm->getSenderId());
            recordScalar("##KeyId", bm->getKeyId());
            recordScalar("##beaconId", bm->getBeaconId());
            recordScalar("##waitingTime", waited);
            if(flag_queue == 0)
            {
                queue_l++;
            }
            else if(flag_queue == 1)
            {
                queue_m++;
            }
            else if(flag_queue == 2)
            {
                queue_h++;
            }
        }
        //EV<<"PC cached, skip"<<"\n";
    }
    else
    {
        delay = msgVrfcDelay;
        bogus_msg_recv--;
        bogus_msg_check++;
        //EV<<"Wrong signature"<<"\n";
    }
    if (!messageVrfd->isScheduled()) {
        scheduleAt(simTime()+delay, messageVrfd);
    }

}

void veins::BeaconAppTESLA::broadcastMessage()
{
    if (!startScalar) {
        recordScalar("##startTime", simTime().dbl());
        recordScalar("##startX", curPosition.x);
        recordScalar("##startY", curPosition.y);
        startScalar = true;
    }
    BeaconMessage* bm = new BeaconMessage();
    if (beaconId == PC_lifetime)
    {
        beaconId = 0;
        KeyId++;
    }
    std::string randomStr = generateRandomString(16);
    bm->setTimestamp(simTime());
    bm->setKind(0);
    bm->setSenderId(nodeId);
    bm->setBeaconId(beaconId++);
    bm->setKeyId(KeyId);
    bm->setSigVrfd(true);
    bm->setPCVrfd(true);
    bm->setHashchainVrfd(true);
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
            if (interestedRange == 0||currentDistTo(tmpPosition)<= interestedRange) {
                sendDirect(bm->dup(), tmpNode->getSubmodule("appl"), "beaconradioIn");
            }
        //}
    }
    recordScalar("##neighborsize", hosts.size());
}

void veins::BeaconAppTESLA::broadcastSolutionMessage()
{
    if (!startScalar) {
        recordScalar("##startTime", simTime().dbl());
        recordScalar("##startX", curPosition.x);
        recordScalar("##startY", curPosition.y);
        startScalar = true;
    }
    BeaconMessage* bm = new BeaconMessage();
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
            if (interestedRange == 0||currentDistTo(tmpPosition)<= interestedRange) {
                sendDirect(bm->dup(), tmpNode->getSubmodule("appl"), "beaconradioIn");
            }
        //}
    }
}

Coord veins::BeaconAppTESLA::getCurPosition() {
    return curPosition;
}
double veins::BeaconAppTESLA::currentDistTo(Coord pos) {
    return currentDistTo(curPosition, pos.x, pos.y);
}

double veins::BeaconAppTESLA::currentDistTo(double x, double y) {
    return currentDistTo(curPosition, x, y);
}

double veins::BeaconAppTESLA::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::BeaconAppTESLA::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void veins::BeaconAppTESLA::finish(){
        recordScalar("##queue_h", queue_h);
        recordScalar("##queue_m", queue_m);
        recordScalar("##queue_l", queue_l);
        recordScalar("##expired_msg", expired_msg);
        recordScalar("##checked_bogus_msg", bogus_msg_check);
        recordScalar("##msg_recv", msg_recv - pendingQueue_h.size() - pendingQueue_m.size() - pendingQueue_l.size() + bogus_msg_recv);
        cancelAndDelete(processQueueEvent);
        cancelAndDelete(broadcastTimer);
        cancelAndDelete(broadcastSolutionTimer);
        cancelAndDelete(messageVrfd);
        while (!pendingQueue_h.empty()) {
            cMessage* m = pendingQueue_h.front();
            pendingQueue_h.pop();
            delete m;
        }
        while (!pendingQueue_m.empty()) {
                    cMessage* m = pendingQueue_m.front();
                    pendingQueue_m.pop();
                    delete m;
                }
        while (!pendingQueue_l.empty()) {
                    cMessage* m = pendingQueue_l.front();
                    pendingQueue_l.pop();
                    delete m;
                }


        /*for (auto& msg : messageList) {
            if (msg) {
                cancelAndDelete(msg);
            }
        }*/
        messageList.clear();
        DemoBaseApplLayer::finish();
}
cMessage* veins::BeaconAppTESLA::findMessageByKeyId(const std::string& keyId) {
    auto it = indexMap.find(keyId);
    if (it == indexMap.end()) {
        return nullptr;
    }
    //return *(it->second);
    return (*(it->second))->dup();
}
bool veins::BeaconAppTESLA::removeMessageByKeyId(const std::string& keyId) {
    auto it = indexMap.find(keyId);
    if (it == indexMap.end()) {
        return false;
    }
    auto listIter = it->second;
    omnetpp::cMessage* msg = *listIter;
    messageList.erase(listIter);
    indexMap.erase(it);
    delete msg;
    return true;
}

std::string veins::BeaconAppTESLA::generateRandomString(size_t length) {
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


