package org.car2x.veins.modules.application.traci;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple BatchVeh extends DemoBaseApplLayer
{
	parameters:
	    //@class("BeaconApp");
	    @class(veins::BatchVeh);
        @display("i=block/app");
        @signal[sentPk](type=long);
        @signal[recvPk](type=long);
        @statistic[pkSent](source=sentPk;record=count);
        @statistic[pkRecv](source=recvPk;record=count);
        double Delay = default(0.001);
        double puzzle = default(0.01);
        double lifeTime @unit(s) = default(1s);
        double interestedRange @unit(m) = default(50m);
        int PC_lifetime = default(600);
        int payloadBytes @unit(B) = default(500B);
        int solpayloadBytes @unit(B) = default(50B);
    gates:
        input beaconradioIn;
}