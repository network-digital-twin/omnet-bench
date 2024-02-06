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
import inet.queueing.classifier.PacketClassifier;
import inet.queueing.scheduler.PriorityScheduler;
import inet.queueing.server.TokenBasedServer;
import inet.queueing.tokengenerator.TimeBasedTokenGenerator;
import inet.queueing.sink.PassivePacketSink;

// ZteNet is the main benchmark network.
network ZteNet
{
    parameters:
        @display("bgb=936.1687,455.56873");
    submodules:
        sender: Terminal {
            @display("p=47.05875,231.28874");
        }
        classifier: PacketClassifier {
            classifierClass = "zte_qos::pipeline::classifier::Tos2QosClassifier";
            @display("p=243.30374,231.28874");
        }
        inChan: PacketDelayer {
            @display("p=145.18124,231.28874");
        }
        priority0: RedQueue {
            @display("p=350.4375,142.17749");
        }
        priority1: RedQueue {
            @display("p=350.4375,231.28874");
        }
        priority2: RedQueue {
            @display("p=350.4375,319.39874");
        }
        scheduler: PriorityScheduler {
            @display("p=504.63,231.28874");
        }
        shaper: TokenBasedServer {
            @display("i=block/bucket;p=665.83124,231.28874");
            @display("p=665.83124,231.28874");
        }
        tokenGen: TimeBasedTokenGenerator {
            storageModule = "^.shaper";
            @display("p=665.83124,84.104996");
        }
        outChan: PacketDelayer {
            @display("p=780.975,231.28874");
        }
        receiver: PassivePacketSink {
            @display("p=890.1112,231.28874");
        }
    connections allowunconnected:
        // sender --> classifier
        sender.out --> inChan.in;
        inChan.out --> classifier.in;
        // classifier --> priority
        classifier.out++ --> priority0.in;
        classifier.out++ --> priority1.in;
        classifier.out++ --> priority2.in;
        // priority --> scheduler
        priority0.out --> scheduler.in++;
        priority1.out --> scheduler.in++;
        priority2.out --> scheduler.in++;
        // scheduler --> shaper
        scheduler.out --> shaper.in;
        // shaper --> receiver
        shaper.out --> outChan.in;
        outChan.out --> receiver.in;
}
