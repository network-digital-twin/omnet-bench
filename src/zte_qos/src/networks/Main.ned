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
import zte_qos.pipeline.QosPipeline;
import inet.queueing.common.PacketDelayer;
import inet.queueing.sink.PassivePacketSink;

// ZteNet is the main benchmark network.
network ZteNet
{
    parameters:
        @display("bgb=830.725,244.4");
    submodules:
        sender: Terminal {
            @display("p=81.075,115.149994");
        }
        inChan: PacketDelayer {
            @display("p=237.34999,115.149994");
        }
        // TODO: move to switch inside a compound module
        qosPipeline: QosPipeline {
            @display("p=411.25,115.149994");
        }
        outChan: PacketDelayer {
            @display("p=568.7,115.149994");
        }
        receiver: PassivePacketSink {
            @display("p=736.725,115.149994");
        }
    connections allowunconnected:
        // sender --> qos
        sender.out --> inChan.in;
        inChan.out --> qosPipeline.in;
        // qos --> receiver
        qosPipeline.out --> outChan.in;
        outChan.out --> receiver.in;
}
