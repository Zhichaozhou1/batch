#include "veins/modules/application/traci/BeaconApp.h"

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif
using namespace veins;
Define_Module(veins::BeaconApp);

static simsignal_t appSentSig = cComponent::registerSignal("sentPk");
static simsignal_t appRecvSig = cComponent::registerSignal("recvPk");
void veins::BeaconApp::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        cachedpsdy = std::vector<int>(20000, -1);
        //nodeId = getParentModule()->getIndex();
        cModule* mobilityModule = getParentModule()->getSubmodule("veinsmobility");
        TraCIMobility* mobility = dynamic_cast<TraCIMobility*>(mobilityModule);
        std::string externalId = mobility->getExternalId();
        auto pos = externalId.find('_');
        //std::string numericPart = externalId.substr(6);
        std::string numericPart = externalId.substr(0, pos);
        //nodeId = std::stoi(numericPart);
        //EV<<nodeId<<"\n";
        nodeId = getParentModule()->getIndex();
        beaconId = 0;
        queue_l = 0;
        queue_m = 0;
        queue_h = 0;
        msg_recv = 0;
        bogus_msg_recv = 0;
        flag_queue = 0;
        expired_msg = 0;
        bogus_msg_check = 0;
        lifeTime = 1;
        braking_time = 0;
        sentCount = 0;
        recvCount = 0;
        processQueueEvent = new cMessage("processQueueEvent");
        broadcastTimer = new cMessage("broadcastTimer");
        geocastTimer = new cMessage("geocastTimer");
        geocasttrans = new cMessage("geocasttrans");
        broadcastSolutionTimer = new cMessage("broadcastSolutionTimer");
        geocastSolutionTimer = new cMessage("geocastSolutionTimer");
        messageVrfd =  new cMessage("messageVrfd");
        braking = new cMessage("braking");
        emergencyannounce = new cMessage("emergencyannounce");
        emannounceSolutionTimer = new cMessage("emannounceSolutionTimer");
        distance = new cMessage("distance");
        certVrfcDelay = par("Delay").doubleValue();
        msgVrfcDelay = par("Delay").doubleValue();
        puzzlesolution = par("puzzle").doubleValue();
        if(simTime().dbl()<23400)
        {
            scheduleAt(23400.5, broadcastTimer);
        }
        else
        {
            scheduleAt(simTime() + 0.15, broadcastTimer);
            if(nodeId%20==0)
            {
                scheduleAt(simTime() + 0.15 + uniform(-0.002,0.002), geocastTimer);
            }
            if(nodeId%51==0)
            {
                scheduleAt(simTime()+5 + uniform(-0.002,0.002), braking);
            }
        }
        beaconradioIn = findGate("beaconradioIn");
    }
}

void veins::BeaconApp::handleMessage(cMessage* msg){

    if (msg->isSelfMessage()) {
            if (msg == processQueueEvent) {
                processPendingQueue();
            }
            else if (msg == geocastTimer)
            {
                BeaconMessage* bm = new BeaconMessage();
                int L=par("payloadBytes").doubleValueInUnit("B");
                bm->setByteLength(L);
                populateWSM(bm);
                std::string randomStr = generateRandomString(16);
                bm->setTimestamp(simTime());
                bm->setTimestamp_ini(simTime());
                bm->setKind(0);
                bm->setGeocast(1);
                bm->setSenderId(nodeId);
                bm->setGeosenderId(nodeId);
                bm->setBeaconId(beaconId++);
                bm->setKeyId(KeyId);
                bm->setSigVrfd(true);
                bm->setPCVrfd(true);
                bm->setHashchainVrfd(true);
                bm->setMsgId(randomStr.c_str());
                bm->setBraking(false);
                geocast(bm);
            }
            else if (msg == broadcastTimer) {
                broadcastMessage();
                //EV<<beaconId<<"\n";
                if(beaconId%1 == 0&&!broadcastSolutionTimer->isScheduled())
                {
                    //scheduleAt(simTime() + puzzlesolution, broadcastSolutionTimer);
                }
                scheduleAt(simTime() + 0.1 + uniform(-0.002,0.002), broadcastTimer);
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
            else if(msg == geocastSolutionTimer)
            {
                geocastSolutionMessage();
            }
            else if(msg == braking)
            {
                emergencybraking();
            }
            else if(msg == emergencyannounce)
            {
                emannounce();
            }
            else if(msg == emannounceSolutionTimer)
            {
                emannounceSolutionMessage();
            }
            else if(msg == distance)
            {
                distancecheck();
            }
            else if(msg == geocasttrans)
            {
                BeaconMessage* bm = messageList4.front();
                messageList4.pop_front();
                geocast(bm);
            }
            else {
                delete msg;
            }
        }
    else {

            BeaconMessage* bm = check_and_cast<BeaconMessage*>(msg);
            int senderId = bm->getSenderId();
            int keyId = bm->getKeyId();
            std::pair<int, int> keyPair(senderId, keyId);
            if (vehiclerecvnum.find(keyPair) == vehiclerecvnum.end()) {
                vehiclerecvnum[keyPair] = bm->getBeaconId();
                vehiclerecvnum_pre[keyPair] = bm->getBeaconId();
            }
            else
            {
                auto it = vehiclerecvnum.find(keyPair);
                if(vehiclerecvnum_pre.find(keyPair) == vehiclerecvnum_pre.end()) {
                    vehiclerecvnum_pre[keyPair] = it->second;
                }
                else
                {
                    auto it2 = vehiclerecvnum_pre.find(keyPair);
                    it2->second=it->second;
                }
                it->second=bm->getBeaconId();
            }

            if(bm->getKind()==0){
                if(uniform(0,1)<packet_loss)
                {
                if(bm->getSigVrfd())
                {
                    /*if(bm->getGeocast()==0)
                    {
                        EV<<"1"<<"\n";
                    }*/
                    msg_recv++;
                    recvCount++;
                    if(bm->getBraking())
                    {
                        if(brakingrecv[keyPair]==1)
                        {
                            delete bm;
                            return;
                        }
                        else
                        {
                            auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
                            auto* vehCmd = mobility->getVehicleCommandInterface();
                            double v = vehCmd->getSpeed();
                            Coord dir = mobility->getCurrentDirection();
                            Coord pos_now = curPosition;
                            if(std::floor(bm->getSendingTime().dbl())-std::floor(simTime().dbl())==0)
                            {
                                pos_now.x = pos_now.x+v*dir.x*(bm->getSendingTime().dbl()-std::floor(bm->getSendingTime().dbl()));
                                pos_now.y = pos_now.y+v*dir.y*(bm->getSendingTime().dbl()-std::floor(bm->getSendingTime().dbl()));
                            }
                            else
                            {
                                pos_now.x = pos_now.x-v*dir.x*(1-(bm->getSendingTime().dbl()-std::floor(bm->getSendingTime().dbl())));
                                pos_now.y = pos_now.y-v*dir.y*(1-(bm->getSendingTime().dbl()-std::floor(bm->getSendingTime().dbl())));
                            }
                            brakingdistance[keyPair] = pos_now;
                        }
                    }
                }
                else
                {
                    bogus_msg_recv++;
                }
                if(cachedpsdy[bm->getSenderId()]==bm->getKeyId()&&bm->getHashchainVrfd())
                {
                    //pendingQueue_m.push(msg);
                    pendingQueue_l.push(msg);
                }
                else{
                    //recordScalar("##message from", senderId);
                    //recordScalar("##keyId", KeyId);
                    //recordScalar("##receive", simTime().dbl());
                    simtime_t ts = bm->getTimestamp();
                    if (vehiclediscovery.find(keyPair) == vehiclediscovery.end()&&bm->getGeocast()==0) {
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
                else
                {
                    delete msg;
                }
            }
            else if(bm->getKind()==1)
            {
                cMessage* foundMsg = findMessageByKeyId(bm->getMsgId());
                //cMessage* foundMsg = foundmsg.get();
                if(!foundMsg)
                {
                    //EV<<"no such message"<<"\n";
                    delete foundMsg;
                    return;
                }
                else{
                    //EV<<"find such message"<<"\n";
                    pendingQueue_h.push(foundMsg);
                    bool removed = removeMessageByKeyId(bm->getMsgId());
                    //EV<<"removed message"<<"\n";
                    messageList2.push_back(msg->dup());
                    auto iter = --messageList2.end();
                    std::string key = bm->getMsgId();
                    movedtoqueuehMap[key] = iter;
                    //EV<<"queued message"<<"\n";
                }
                delete msg;
            }

    }
}

void veins::BeaconApp::processPendingQueue() {
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
                delete wsm;
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
                delete wsm;
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
            //bool removed = removeMessageByKeyId(bm->getMsgId());
            int senderId = bm->getSenderId();
            int keyId = bm->getKeyId();
            if (bm->getGeocast()==0)
            {
                if(removeMessagefromdoubleqlByKeyId(bm->getMsgId()))
                {
                    delete msg;
                    return;
                }
                std::pair<int, int> keyPair(senderId, keyId);
                auto it = vehiclestatus.find(keyPair);
                if(it != vehiclestatus.end())
                {
                    simtime_t time = it->second;
                    if(time.dbl()>bm->getTimestamp().dbl())
                    {
                        delete msg;
                        return;
                    }
                }
            }
            if (wsm) {
                flag_queue = 0;
                //queue_l++;
                //recordScalar("##Go to Queue_l", 1);
                //recordScalar("##Queue_h length", pendingQueue_h.size());
                //recordScalar("##Queue_m length", pendingQueue_m.size());
                //recordScalar("##Queue_l length", pendingQueue_l.size());
                onWSM(wsm);
                delete wsm;
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

void veins::BeaconApp::onWSM(BaseFrame1609_4* frame){
    BeaconMessage* bm = check_and_cast<BeaconMessage*>(frame);
    simtime_t now = simTime();
    double delay = 0;
    double waited = 0;
    double discover_waited = 0;
    int senderId = bm->getSenderId();
    int keyId = bm->getKeyId();
    //bool removed = removeMessageByKeyId(bm->getMsgId());
    if ((now.dbl() - bm->getTimestamp()).dbl() > lifeTime && !bm->getBraking())
    {
        double time_now=now.dbl();
        double time_send=bm->getTimestamp().dbl();
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
    std::pair<int, int> keyPair2(senderId, keyId);
    auto it2 = vehiclestatus.find(keyPair2);
    if (bm->getGeocast()==0&&it2 != vehiclestatus.end())
    {
        if(it2->second.dbl()>bm->getTimestamp().dbl())
        {
            if (!messageVrfd->isScheduled()) {
                scheduleAt(simTime()+delay, messageVrfd);
            }
            return;
        }
    }
    auto it = geocastMap.find(bm->getMsgId());
    int geocastindex=bm->getGeocast();
    if(it!=geocastMap.end())
    {
        if (!messageVrfd->isScheduled()) {
            scheduleAt(simTime()+delay, messageVrfd);
        }
        return;
    }
    if(bm->getBraking())
        {
            std::pair<int, int> keyPair(senderId, keyId);
            if(brakingrecv[keyPair]==1)
            {
                if (!messageVrfd->isScheduled()) {
                    scheduleAt(simTime()+delay, messageVrfd);
                }
                return;
            }
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
            //if(curPosition.x>13000&&curPosition.y>13000&&curPosition.x<14000&&curPosition.y<14000)
            //{
                //recordScalar("##signerId", bm->getSenderId());
                //recordScalar("##KeyId", bm->getKeyId());
                //recordScalar("##beaconId", bm->getBeaconId());
                //recordScalar("##first receive", storedTimestamp.dbl());
                //recordScalar("##process at", simTime().dbl());
            //}
            auto it4 = vehiclerecvnum_pre.find(keyPair);
            if(nodeId%1==0&&geocastindex==0&&!bm->getBraking())
            {
                if(bm->getBeaconId()-1<=it4->second&&currentDistTo(bm->getSenderPos())<= 180)
                {
                    recordScalar("##waitingTime", waited);
                    recordScalar("##discoverwaitingTime", discover_waited);
                    recordScalar("##discoverin", flag_queue);
                }
            }
            //std::pair<int, int> keyPair2(senderId, keyId);
            //auto it2 = vehiclestatus.find(keyPair2);
            //auto it3 = vehiclerecv.find(keyPair2);
            if (bm->getGeocast()==0)
            {
                if(it2 != vehiclestatus.end())
                {
                    simtime_t time = it2->second;
                    //simtime_t timerecv = it3->second;
                    if(bm->getBeaconId()-1<=it4->second&&!bm->getBraking()&&currentDistTo(bm->getSenderPos())<150)
                    {
                        recordScalar("##updateTime", delay+bm->getTimestamp().dbl()-time.dbl());

                    }
                    it2->second = simTime();
                }
                else
                {
                    if(!bm->getBraking()&&currentDistTo(bm->getSenderPos())<150)
                    {
                        vehiclestatus.emplace(keyPair2,simTime());
                    }
                    //if(nodeId%1==0 && bm->getBeaconId()<=it4->second&&!bm->getBraking())
                    //{
                    //    recordScalar("##updateTime", discover_waited);
                    //}
                }
                //vehiclerecv[keyPair] = bm->getTimestamp();
            }
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
        //if(flag_queue == 0)
        //{

        //}
        //else
       // {
            int senderId = bm->getSenderId();
            int keyId = bm->getKeyId();
            delay = msgVrfcDelay;
            waited = simTime().dbl()-bm->getTimestamp().dbl()+delay;
            std::pair<int, int> keyPair(senderId, keyId);
            auto it4 = vehiclerecvnum_pre.find(keyPair);
            //if(curPosition.x>13000&&curPosition.y>13000&&curPosition.x<14000&&curPosition.y<14000)
            //{
                //recordScalar("##signerId", bm->getSenderId());
                //recordScalar("##KeyId", bm->getKeyId());
                //recordScalar("##beaconId", bm->getBeaconId());
            if(nodeId%1==0&&geocastindex==0&&!bm->getBraking())
            {
                double distance = currentDistTo(bm->getSenderPos());
                recordScalar("##waitingTime", waited);

            }
            if(bm->getGeocast()==0)
            {
                std::pair<int, int> keyPair2(senderId, keyId);
                auto it2 = vehiclestatus.find(keyPair2);
                //auto it3 = vehiclerecv.find(keyPair2);
                if(it2 != vehiclestatus.end())
                {
                    simtime_t time = it2->second;
                    //simtime_t timerecv = it3->second;
                    if(bm->getBeaconId()-1<=it4->second&&!bm->getBraking()&&currentDistTo(bm->getSenderPos())<150)
                    {
                        recordScalar("##updateTime", delay+bm->getTimestamp().dbl()-time.dbl());

                    }
                    it2->second = simTime();
                }
                else
                {
                    if(!bm->getBraking()&&currentDistTo(bm->getSenderPos())<150)
                    {
                        vehiclestatus.emplace(keyPair2,simTime());
                    }
                    //if(nodeId%1==0 && bm->getBeaconId()<=it4->second&&!bm->getBraking())
                    //{
                    //   recordScalar("##updateTime", discover_waited);
                    //}
                }
                //vehiclerecv[keyPair2] = bm->getTimestamp();
            }
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
        //}
        //EV<<"PC cached, skip"<<"\n";
    }
    else
    {
        delay = msgVrfcDelay;
        bogus_msg_recv--;
        bogus_msg_check++;
        //EV<<"Wrong signature"<<"\n";
    }
    if(geocastindex>0)
    {
        if(nodeId%1==0)
        {
            recordScalar("##geocast", waited+0.003);
            recordScalar("##hop", bm->getGeocast());
            recordScalar("##geofrom", bm->getGeosenderId());
            recordScalar("##geocasttrans",simTime().dbl()-bm->getTimestamp_ini().dbl()+delay+0.003);
        }
        std::string key = bm->getMsgId();
        messageList3.push_back(bm->dup());
        messageList4.push_back(bm->dup());
        auto iter = --messageList3.end();
        geocastMap[key] = iter;
        //geocast(bm);
        delay = delay+0.003;
        if(!geocasttrans->isScheduled())
        {
            scheduleAt(simTime()+delay + uniform(-0.002,0.002), geocasttrans);
        }
    }
    if(bm->getBraking())
    {
        std::pair<int, int> keyPair(senderId, keyId);
        if(brakingrecv[keyPair]!=1)
        {
            brakingrecv[keyPair]=1;
            braking_time = bm->getTimestamp().dbl();
            pos_cache = bm->getSenderPos();
            keyId_cache = keyId;
            senderId_cache = senderId;
            scheduleAt(simTime()+delay-0.00001, distance);
        }
    }
    if (!messageVrfd->isScheduled()) {
        scheduleAt(simTime()+delay, messageVrfd);
    }

}

void veins::BeaconApp::broadcastMessage()
{
    int neighbor=0;
    if (!startScalar) {
        //recordScalar("##startTime", simTime().dbl());
        //recordScalar("##startX", curPosition.x);
        //recordScalar("##startY", curPosition.y);
        startScalar = true;
    }
    BeaconMessage* bm = new BeaconMessage();
    populateWSM(bm);
    int L=par("payloadBytes").doubleValueInUnit("B");
    bm->setByteLength(L);
    if (beaconId == 10*PC_lifetime)
    {
        beaconId = 0;
        KeyId++;
    }
    std::string randomStr = generateRandomString(16);
    bm->setTimestamp(simTime());
    bm->setKind(0);
    bm->setGeocast(0);
    bm->setSenderId(nodeId);
    bm->setBeaconId(beaconId++);
    bm->setKeyId(KeyId);
    bm->setSigVrfd(true);
    bm->setPCVrfd(true);
    bm->setHashchainVrfd(true);
    bm->setMsgId(randomStr.c_str());
    bm->setSenderPos(curPosition);
    bm->setBraking(false);
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
                sentCount++;
            }
        //}
    }

    //sendDown(bm->dup());
    delete bm;
    //recordScalar("##neighborsize", neighbor);
}


void veins::BeaconApp::geocast(BeaconMessage* bm)
{
    geoMsgId = bm->getMsgId();
    int geocast = bm->getGeocast();
    bm->setGeocast(geocast+1);
    bm->setTimestamp(simTime());
    bm->setKeyId(KeyId);
    bm->setSenderId(nodeId);
    populateWSM(bm);
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
                sentCount++;
            }
        //}
    }
    //sendDown(bm);

    if(!geocastSolutionTimer->isScheduled())
    {
        //scheduleAt(simTime() + puzzlesolution, geocastSolutionTimer);
    }
    //recordScalar("##neighborsize", neighbor);
}

void veins::BeaconApp::broadcastSolutionMessage()
{
    if (!startScalar) {
        //recordScalar("##startTime", simTime().dbl());
        //recordScalar("##startX", curPosition.x);
        //recordScalar("##startY", curPosition.y);
        startScalar = true;
    }
    BeaconMessage* bm = new BeaconMessage();
    populateWSM(bm);
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
            if (interestedRange == 0||currentDistTo(tmpPosition)<= interestedRange) {
                sendDirect(bm->dup(), tmpNode->getSubmodule("appl"), "beaconradioIn");
            }
        //}
    }
    //sendDown(bm->dup());
    delete bm;
}

void veins::BeaconApp::geocastSolutionMessage()
{
    BeaconMessage* bm = new BeaconMessage();
    populateWSM(bm);
    int L=par("solpayloadBytes").doubleValueInUnit("B");
    bm->setByteLength(L);
    bm->setTimestamp(simTime());
    bm->setKind(1);
    bm->setSenderId(nodeId);
    bm->setPuzzleVrfd(true);
    bm->setMsgId(geoMsgId.c_str());
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

    //sendDown(bm->dup());
    delete bm;
}

void veins::BeaconApp::emergencybraking()
{
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    auto* vehCmd = mobility->getVehicleCommandInterface();
    double v = vehCmd->getSpeed();
    if(v>0)
    {
        vehCmd->slowDown(0.0, 1);
        braking_at = simTime();
        braking_pos = mobility->getPositionAt(simTime());
        scheduleAt(simTime(), emergencyannounce);
    }
}

void veins::BeaconApp::emannounce()
{
    BeaconMessage* bm = new BeaconMessage();
    populateWSM(bm);
    int L=par("payloadBytes").doubleValueInUnit("B");
    bm->setByteLength(L);
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    auto* vehCmd = mobility->getVehicleCommandInterface();
    std::string randomStr = generateRandomString(16);
    bm->setTimestamp(braking_at);
    bm->setKind(0);
    bm->setGeocast(0);
    bm->setSenderId(nodeId);
    bm->setBeaconId(beaconId++);
    bm->setKeyId(KeyId);
    bm->setSigVrfd(true);
    bm->setPCVrfd(true);
    bm->setHashchainVrfd(true);
    bm->setMsgId(randomStr.c_str());
    bm->setKeyId(KeyId);
    bm->setSenderId(nodeId);
    bm->setSenderPos(braking_pos);
    bm->setBraking(true);
    announceId = bm->getMsgId();

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
            sentCount++;
        }
            //}
    }
    //sendDown(bm->dup());
    if(repeat_time!=0)
    {
        repeat_time--;
        scheduleAt(simTime()+0.01, emergencyannounce);
    }
    /*if(!emannounceSolutionTimer->isScheduled())
    {
        if(repeat_time==29)
         {
             scheduleAt(simTime() + puzzlesolution, emannounceSolutionTimer);
         }
         else
         {
             scheduleAt(simTime() + 0.001, emannounceSolutionTimer);
         }

    }*/
    delete bm;
}

void veins::BeaconApp::emannounceSolutionMessage()
{
    BeaconMessage* bm = new BeaconMessage();
    populateWSM(bm);
    int L=par("solpayloadBytes").doubleValueInUnit("B");
    bm->setByteLength(L);
    bm->setTimestamp(simTime());
    bm->setKind(1);
    bm->setSenderId(nodeId);
    bm->setPuzzleVrfd(true);
    bm->setMsgId(announceId.c_str());
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
    //sendDown(bm->dup());
    delete bm;
}

void veins::BeaconApp::distancecheck()
{
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    auto* vehCmd = mobility->getVehicleCommandInterface();
    double v = vehCmd->getSpeed();
    Coord dir = mobility->getCurrentDirection();
    Coord distance_before;
    std::pair<int, int> keyPair(senderId_cache, keyId_cache);
    auto it = brakingdistance.find(keyPair);
    if(it != brakingdistance.end())
    {
        distance_before = it->second;
    }
    double time_d = simTime().dbl()-braking_time;
    double time_diff = time_d-std::floor(time_d);
    //double distance_now = currentDistTo(curPosition.x+v*dir.x*time_diff,curPosition.y+v*dir.y*time_diff,distance_before.x,distance_before.y);
    double distance_now = v*time_d;
    if(distance_now>0.001)
    {
        recordScalar("##speed",v);
        recordScalar("##distance_diff", distance_now);
    }
}

Coord veins::BeaconApp::getCurPosition() {
    return curPosition;
}
double veins::BeaconApp::currentDistTo(Coord pos) {
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    return currentDistTo(mobility->getPositionAt(simTime()), pos.x, pos.y);
}

double veins::BeaconApp::currentDistTo(double x, double y) {
    auto* mobility = veins::TraCIMobilityAccess().get(getParentModule());
    return currentDistTo(mobility->getPositionAt(simTime()), x, y);
}

double veins::BeaconApp::currentDistTo(Coord pos, double x, double y) {
    return currentDistTo(pos.x, pos.y, x, y);
}

double veins::BeaconApp::currentDistTo(double x1, double y1, double x2, double y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void veins::BeaconApp::finish(){
        //recordScalar("##checked_bogus_msg", bogus_msg_check);
        //recordScalar("##msg_recv", msg_recv - pendingQueue_h.size() - pendingQueue_m.size() - pendingQueue_l.size() + bogus_msg_recv);
        cancelAndDelete(processQueueEvent);
        cancelAndDelete(broadcastTimer);
        cancelAndDelete(broadcastSolutionTimer);
        cancelAndDelete(messageVrfd);
        cancelAndDelete(geocastTimer);
        cancelAndDelete(geocastSolutionTimer);
        while (!pendingQueue_h.empty()) {
            cMessage* m = pendingQueue_h.front();
            BeaconMessage* bm = check_and_cast<BeaconMessage*>(m);
            simtime_t now = simTime();
            if ((now.dbl() - bm->getTimestamp()).dbl() > lifeTime && !bm->getBraking())
            {
                if(!bm->getSigVrfd())
                {
                    bogus_msg_recv--;
                }
                else
                {
                    expired_msg++;
                }
            }
            pendingQueue_h.pop();
            delete m;
        }
        while (!pendingQueue_m.empty()) {
            cMessage* m = pendingQueue_m.front();
            BeaconMessage* bm = check_and_cast<BeaconMessage*>(m);
            simtime_t now = simTime();
            if ((now.dbl() - bm->getTimestamp()).dbl() > lifeTime && !bm->getBraking())
            {
                if(!bm->getSigVrfd())
                {
                    bogus_msg_recv--;
                }
                else
                {
                    expired_msg++;
                }
            }
            pendingQueue_m.pop();
            delete m;
        }
        while (!pendingQueue_l.empty()) {
            cMessage* m = pendingQueue_l.front();
            BeaconMessage* bm = check_and_cast<BeaconMessage*>(m);
            simtime_t now = simTime();
            if ((now.dbl() - bm->getTimestamp()).dbl() > lifeTime && !bm->getBraking())
            {
                if(!bm->getSigVrfd())
                {
                    bogus_msg_recv--;
                }
                else
                {
                    expired_msg++;
                }
            }
            pendingQueue_l.pop();
            delete m;
        }


        /*for (auto& msg : messageList) {
            if (msg) {
                cancelAndDelete(msg);
            }
        }*/
        for(auto ptr:messageList)
        {
            delete ptr;
        }
        for(auto ptr2:messageList2)
        {
            delete ptr2;
        }
        for(auto ptr3:messageList3)
        {
            delete ptr3;
        }
        for(auto ptr4:messageList4)
        {
            delete ptr4;
        }
        messageList.clear();
        messageList2.clear();
        messageList3.clear();
        messageList4.clear();
        indexMap.clear();
        movedtoqueuehMap.clear();
        geocastMap.clear();
        recordScalar("##queue_h", queue_h);
        recordScalar("##queue_m", queue_m);
        recordScalar("##queue_l", queue_l);
        recordScalar("##expired_msg", expired_msg);
        recordScalar("##sent", sentCount);
        recordScalar("##recv", recvCount);
        DemoBaseApplLayer::finish();
}
cMessage* veins::BeaconApp::findMessageByKeyId(const std::string& keyId) {
    auto it = indexMap.find(keyId);
    if (it == indexMap.end()) {
        return nullptr;
    }
    //return *(it->second);
    return (*(it->second))->dup();
}
bool veins::BeaconApp::removeMessageByKeyId(const std::string& keyId) {
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

bool veins::BeaconApp::removeMessagefromdoubleqlByKeyId(const std::string& keyId) {
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

std::string veins::BeaconApp::generateRandomString(size_t length) {
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


