//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
package org.car2x.veins.modules.application.traci;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple BeaconApp extends DemoBaseApplLayer
{
	parameters:
	    //@class("BeaconApp");
	    @class(veins::BeaconApp);
        @display("i=block/app");
        @signal[sentPk](type=long);
        @signal[recvPk](type=long);
        @statistic[pkSent](source=sentPk;record=count);
        @statistic[pkRecv](source=recvPk;record=count);
        double Delay = default(0.001);
        double puzzle = default(0.02);
        double lifeTime @unit(s) = default(1s);
        double interestedRange @unit(m) = default(50m);
        int PC_lifetime = default(600);
        int payloadBytes @unit(B) = default(500B);
        int solpayloadBytes @unit(B) = default(50B);
    gates:
        input beaconradioIn;
}