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

simple BeaconAppTESLA extends DemoBaseApplLayer
{
	parameters:
	    //@class("BeaconApp");
	    @class(veins::BeaconAppTESLA);
        @display("i=block/app");
        double certVrfcDelay @unit(s) = default(0.0015s);
        double msgVrfcDelay @unit(s) = default(0.0015s);
        double lifeTime @unit(s) = default(1s);
        double interestedRange @unit(m) = default(50m);
        int PC_lifetime = default(600);
    gates:
        input beaconradioIn;
}