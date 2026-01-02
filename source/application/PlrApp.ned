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

simple PlrApp extends DemoBaseApplLayer
{
    parameters:
        @class(veins::PlrApp);
		double sendFreq @unit(Hz) = default(10Hz); // ← 频率（Hz）
        double sendInterval @unit(s) = default(0.1s); // 10 Hz
        int payloadBytes @unit(B) = default(500B); // 500B
        @signal[sentPk](type=long);
        @signal[recvPk](type=long);
        @statistic[pkSent](source=sentPk; record=count);
        @statistic[pkRecv](source=recvPk; record=count);
}