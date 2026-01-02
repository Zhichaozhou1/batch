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

//
// TODO generated message class
//
cplusplus {{
#include "omnetpp.h"
}}

import veins.base.utils.Coord;
import veins.modules.messages.BaseFrame1609_4;

struct MsgKeyPair
{
	int senderId;
	int keyId;
}

packet BatchVeri extends veins::BaseFrame1609_4{
    int kind; // 0:info message, 1:puzzle solution, 2:from RSU include the challenge
    int senderId;
    simtime_t timestamp = 0;
    int beaconId;
    int KeyId;
    bool sigVrfd = false;
    bool PCVrfd = false;
    string MsgId;
    bool hashchainVrfd = false;
    bool puzzleVrfd = false;
    MsgKeyPair pairs[];
}
