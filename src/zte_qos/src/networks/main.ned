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
import zte_qos.pipeline.RedQueue;
import inet.queueing.common.PacketDelayer;
import inet.queueing.server.TokenBasedServer;
import inet.queueing.tokengenerator.TimeBasedTokenGenerator;
import inet.queueing.sink.PassivePacketSink;

// ZteNet is the main benchmark network.
network ZteNet
{
    parameters:
        @display("bgb=797,456");
    submodules:
        sender: Terminal {
            @display("p=68,232");
        }
        inChan: PacketDelayer {
            @display("p=190,232");
        }
        priority: RedQueue {
            @display("p=321,232");
        }
        shaper: TokenBasedServer {
            @display("p=461,232");
        }
        tokenGen: TimeBasedTokenGenerator {
            storageModule = "^.shaper";
            @display("p=461,85");
        }
        outChan: PacketDelayer {
            @display("p=594,232");
        }
        receiver: PassivePacketSink {
            @display("p=726,232");
        }
    connections allowunconnected:
        // sender --> priority
        sender.out --> inChan.in;
        inChan.out --> priority.in;
        // priority --> shaper
        priority.out --> shaper.in;
        // shaper --> receiver
        shaper.out --> outChan.in;
        outChan.out --> receiver.in;
}
