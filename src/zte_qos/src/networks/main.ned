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
import inet.queueing.queue.PacketQueue;
//import inet.queueing.shaper.TokenBucket;

// DummyLine is a dummy connection with a static datarate and transmission delay set, for testing.
channel DummyLine extends ned.DatarateChannel
{
    datarate = 100Mbps;
    delay = 100us;
}

// ZteNet is the main benchmark network.
network ZteNet
{
    @display("bgb=729,466");
    submodules:
        sender: Terminal {
            @display("p=120,134");
        }
        queue: PacketQueue {
            @display("p=302,256");
        }
    connections allowunconnected:
        sender.out --> DummyLine --> queue.in;
}
