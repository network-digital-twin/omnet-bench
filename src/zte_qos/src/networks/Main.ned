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

package zte_qos.networks;

import zte_qos.terminal.Terminal;
import zte_qos.switch.Switch;
import inet.queueing.common.PacketDelayer;
import inet.queueing.sink.PassivePacketSink;

// ZteNet is the main benchmark network.
network ZteNet
{
    submodules:
        sender[2]: Terminal {
            @display("p=81.075,115.149994,c,100");
        }
        inChan: PacketDelayer {
            @display("p=237.34999,115.149994");
        }
        switch: Switch {
            @display("p=411.25,115.149994");
        }
        outChan: PacketDelayer {
            @display("p=568.7,115.149994");
        }
        receiver[2]: PassivePacketSink {
            @display("p=736.725,115.149994,c,100");
        }
    connections allowunconnected:
        // sender --> switch
        sender[0].out --> inChan.in;
        inChan.out --> switch.in++;
        sender[1].out --> switch.in++;
        // switch --> receiver
        switch.out++ --> outChan.in;
        outChan.out --> receiver[0].in;
        switch.out++ --> receiver[1].in;
}
