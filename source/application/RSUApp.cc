#include "veins/modules/application/traci/RSUApp.h"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
using namespace veins;
Define_Module(veins::RSUApp);

void veins::RSUApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        //nodeId = getParentModule()->getIndex();
        processQueueEvent = new cMessage("processQueueEvent");
        broadcastTimer = new cMessage("broadcastTimer");
        neighbordiscovery = new cMessage("neighbordiscovery");
        nodeId = getParentModule()->getId();
        nodeId_dis = nodeId;
        keyId_dis = 0;
        lifeTime = par("lifeTime").doubleValue();
        certVrfcDelay = par("Delay").doubleValue();
        msgVrfcDelay = par("Delay").doubleValue();
        batchinterval = par("batchinterval").doubleValue();
        if(simTime().dbl()<23400)
        {
            scheduleAt(23400.5, broadcastTimer);
            scheduleAt(23400.1, processQueueEvent);
        }
        else
        {
            scheduleAt(simTime() + 0.15, broadcastTimer);
            scheduleAt(simTime(), processQueueEvent);
        }
        beaconradioIn = findGate("beaconradioIn");
    }
}

void veins::RSUApp::handleMessage(cMessage* msg){

    if (msg->isSelfMessage()) {
        if (msg == broadcastTimer) {
            broadcastMessage();
            scheduleAt(simTime() + 1 + uniform(-0.002,0.002), broadcastTimer);
        }
        else if (msg == processQueueEvent) {
            processPendingQueue();
            if(batch_delay<batchinterval)
            {
                if (!processQueueEvent->isScheduled()) {
                    scheduleAt(simTime()+batchinterval, processQueueEvent);
                }
                if (!neighbordiscovery->isScheduled()&&!pendingQueue_l.empty()) {
                    scheduleAt(simTime()+batch_delay,neighbordiscovery);
                }
            }
            else
            {
                if (!processQueueEvent->isScheduled()) {
                    scheduleAt(simTime()+batch_delay, processQueueEvent);
                }
            }
        }
        else if(msg == neighbordiscovery)
        {
            auto it = cachedpsdy.find(nodeId_dis);
            if(it==cachedpsdy.end())
            {
                cachedpsdy.emplace(nodeId_dis,keyId_dis);
            }
            else if(it->second<keyId_dis)
            {
                it->second=keyId_dis;
            }
            discover();
            if (!neighbordiscovery->isScheduled()&&(!pendingQueue_l.empty()||!pendingQueue_h.empty())) {
                scheduleAt(simTime()+dis_delay,neighbordiscovery);
            }
        }
        else
        {
            delete msg;
        }
    }
    else {
        BatchVeri* bm = check_and_cast<BatchVeri*>(msg);
        int senderId = bm->getSenderId();
        int keyId = bm->getKeyId();
        std::pair<int, int> keyPair(senderId, keyId);
        if(bm->getKind()==0){
            if(uniform(0,1)<packet_loss)
            {
                auto it = cachedpsdy.find(senderId);
                if(it!=cachedpsdy.end()&&it->second>=keyId&&bm->getHashchainVrfd())
                {
                    pendingQueue_m.push(msg);
                    //pendingQueue_l.push(msg);
                }
                else
                {
                    pendingQueue_l.push(msg);
                    simtime_t ts = bm->getTimestamp();
                    if (vehiclediscovery.find(keyPair) == vehiclediscovery.end())
                    {
                        vehiclediscovery[keyPair] = ts;
                    }
                    messageList.push_back(msg->dup());
                    auto iter = --messageList.end();
                    std::string key = bm->getMsgId();
                    indexMap[key] = iter;
                    if(!neighbordiscovery->isScheduled())
                    {
                        //scheduleAt(simTime()+0.001,neighbordiscovery);
                    }
                }
                if (!processQueueEvent->isScheduled()) {
                    scheduleAt(simTime(), processQueueEvent);
                }
             }
        }
        else if(bm->getKind()==1)//from RSU
        {
            for(int i=0;i<bm->getPairsArraySize();++i)
            {
                auto p = bm->getPairs(i);
                auto it = cachedpsdy.find(senderId);
                if(it==cachedpsdy.end())
                {
                    cachedpsdy.emplace(p.senderId,p.keyId);
                    //keys.add(p.senderId,p.keyId,true);
                    Dis_from_RSU++;
                }
                else if(it->second<p.keyId)
                {
                    it->second=p.keyId;
                    //keys.add(p.senderId,p.keyId,true);
                    Dis_from_RSU++;
                }
            }
        }
        else if(bm->getKind()==2)
        {
            cMessage* foundMsg = findMessageByKeyId(bm->getMsgId());
            if(!foundMsg)
            {
                delete foundMsg;
                return;
            }
            else
            {
                pendingQueue_h.push(foundMsg);
                bool removed = removeMessageByKeyId(bm->getMsgId());
                messageList2.push_back(msg->dup());
                auto iter = --messageList2.end();
                std::string key = bm->getMsgId();
                movedtoqueuehMap[key] = iter;
            }
        }
    }
}

void veins::RSUApp::discover()
{
    simtime_t now = simTime();
    int flag = 0;
    double delay = 0;
    double waited = 0;
    double discover_waited = 0;
    dis_delay = 0;
    cMessage* msg;
    if(!pendingQueue_h.empty())
    {
        msg = pendingQueue_h.front();
        pendingQueue_h.pop();
        flag=1;
    }
    else if(!pendingQueue_l.empty())
    {
        msg = pendingQueue_l.front();
        pendingQueue_l.pop();
        flag=2;
        BatchVeri* bm = check_and_cast<BatchVeri*>(msg);
        if(removeMessagefromdoubleqlByKeyId(bm->getMsgId()))
        {
            delete msg;
            return;
        }
    }
    BatchVeri* bm = check_and_cast<BatchVeri*>(msg);
    int senderId = bm->getSenderId();
    int keyId = bm->getKeyId();
    std::pair<int, int> keyPair(senderId, keyId);
    auto it = cachedpsdy.find(senderId);
    if ((now.dbl() - bm->getTimestamp()).dbl() > lifeTime)
    {
        double time_now=now.dbl();
        double time_send=bm->getTimestamp().dbl();
        if(!bm->getSigVrfd())
        {

        }
        else
        {
            expired_msg++;
        }
        return;
    }
    else if(it==cachedpsdy.end()||it->second<keyId)
    {
        if(bm->getPCVrfd())
        {
            nodeId_dis = senderId;
            keyId_dis = keyId;
            if(bm->getSigVrfd())
            {
                delay = certVrfcDelay+msgVrfcDelay;
                waited = now.dbl()-bm->getTimestamp().dbl()+delay;
            }
            else
            {
                delay = certVrfcDelay+msgVrfcDelay;
            }
            auto it = vehiclediscovery.find(keyPair);
            simtime_t storedTimestamp = it->second;
            discover_waited = now.dbl()-storedTimestamp.dbl()+delay;
            recordScalar("##discoverwaitingTime", discover_waited);
            keys.add(senderId,keyId,true);
            if(flag==1)
            {
                Dis_from_Qh++;
            }
            else if(flag==2)
            {
                Dis_from_Ql++;
            }
        }
        else
        {
            nodeId_dis = nodeId;
            keyId_dis = 0;
            delay = certVrfcDelay;
        }
    }
    dis_delay = delay;
}

void veins::RSUApp::broadcastMessage()
{
    BatchVeri* bm = new BatchVeri();
    bm->setTimestamp(simTime());
    bm->setKind(1);
    bm->setSenderId(nodeId);
    bm->setBeaconId(beaconId++);
    bm->setPairsArraySize(keys.size());
    int i = 0;
    for (const auto& p : keys.items()) {   // p 是你自定义的 KeyPair
        MsgKeyPair kp;                        // ← 用 .msg 生成的类型
        kp.senderId=p.senderId;
        kp.keyId=p.keyId;
        bm->setPairs(i++, kp);             // 5.x 接口：逐个 set
    }
    keys.clear();
    for (int i = 0; ; ++i) {
        cModule* rsu = getSystemModule()->getSubmodule("rsu", i);
        if (!rsu) break;
        cModule* rsuApp = rsu->getSubmodule("appl");
        if (!rsuApp) continue;
        sendDirect(bm->dup(), rsuApp, "beaconradioIn");
    }
}

void veins::RSUApp::processPendingQueue()
{
    int number = 0;
    double delay = 0;
    double waited = 0;
    if(neighbordiscovery->isScheduled())
    {
        cancelEvent(neighbordiscovery);
    }
    if (!pendingQueue_m.empty())
    {
        number = pendingQueue_m.size();
        delay = (number+2)*msgVrfcDelay/2;
    }
    while (!pendingQueue_m.empty()) {
        cMessage* m = pendingQueue_m.front();
        BatchVeri* bm = check_and_cast<BatchVeri*>(m);
        simtime_t now = simTime();
        if ((now.dbl() - bm->getTimestamp()).dbl() > lifeTime)
        {
            expired_msg++;
        }
        waited = simTime().dbl()-bm->getTimestamp().dbl()+delay;
        recordScalar("##waitingTime", waited);
        pendingQueue_m.pop();
    }
    /*if (!pendingQueue_l.empty())
    {
        number = pendingQueue_m.size();
        delay = (number+2)*msgVrfcDelay/2;
    }*/

    batch_delay = delay;
}

Coord veins::RSUApp::getCurPosition() {
    return curPosition;
}
double veins::RSUApp::currentDistTo(Coord pos) {
    return currentDistTo(curPosition, pos.x, pos.y);
}

double veins::RSUApp::currentDistTo(double x, double y) {
    return currentDistTo(curPosition, x, y);
}

double veins::RSUApp::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::RSUApp::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void veins::RSUApp::finish(){
    cancelAndDelete(processQueueEvent);
    cancelAndDelete(broadcastTimer);
    int rest=0;
    while (!pendingQueue_m.empty()) {
        pendingQueue_m.pop();
    }
    while (!pendingQueue_l.empty()) {
        pendingQueue_l.pop();
        rest++;
    }
    recordScalar("##queue_h", Dis_from_Qh);
    recordScalar("##queue_m", Dis_from_RSU);
    recordScalar("##queue_l", Dis_from_Ql);
    recordScalar("##expired_msg", expired_msg);
    recordScalar("##rest", rest);
    DemoBaseApplLayer::finish();
}


std::string veins::RSUApp::generateRandomString(size_t length) {
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

cMessage* veins::RSUApp::findMessageByKeyId(const std::string& keyId) {
    auto it = indexMap.find(keyId);
    if (it == indexMap.end()) {
        return nullptr;
    }
    //return *(it->second);
    return (*(it->second))->dup();
}
bool veins::RSUApp::removeMessageByKeyId(const std::string& keyId) {
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

bool veins::RSUApp::removeMessagefromdoubleqlByKeyId(const std::string& keyId) {
    auto it = movedtoqueuehMap.find(keyId);
    if (it == movedtoqueuehMap.end()) {
        return false;
    }
    auto listIter = it->second;
    omnetpp::cMessage* msg = *listIter;
    messageList2.erase(listIter);
    movedtoqueuehMap.erase(it);
    delete msg;
    return true;
}
