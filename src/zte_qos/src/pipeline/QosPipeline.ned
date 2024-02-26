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

package zte_qos.pipeline;

import zte_qos.pipeline.RedQueue;
import inet.queueing.classifier.PacketClassifier;
import inet.queueing.scheduler.PriorityScheduler;
import inet.queueing.server.TokenBasedServer;
import inet.queueing.tokengenerator.TimeBasedTokenGenerator;

module QosPipeline
{
    parameters:
        // output port bandwidth
        double bw @unit(bps) = default(0bps);
        // srTCM
        double committedInformationRate @unit(bps);	// TODO: so strange...
        // shaper and tokenGen
        double tokenConsumptionPerBit = default(0);
        double generationInterval @unit(s);
        double numTokens = default(1);
        @display("bgb=689,450;i=block/transport");
    gates:
        input in;
        output out;
    submodules:
        classifier: PacketClassifier {
            classifierClass = "zte_qos::pipeline::classifier::Tos2QosClassifier";
            @display("p=109,231");
        }
        priority0: RedQueue {
            srTcm.committedInformationRate = parent.committedInformationRate;
            @display("p=262,142");
        }
        priority1: RedQueue {
            srTcm.committedInformationRate = parent.committedInformationRate;
            @display("p=262,231");
        }
        priority2: RedQueue {
            srTcm.committedInformationRate = parent.committedInformationRate;
            @display("p=262,319");
        }
        scheduler: PriorityScheduler {
            @display("p=416,231");
        }
        shaper: TokenBasedServer {
            tokenConsumptionPerBit = parent.tokenConsumptionPerBit;
            @display("i=block/bucket;p=577,231");
        }
        tokenGen: TimeBasedTokenGenerator {
            storageModule = "^.shaper";
            generationInterval = parent.generationInterval;
            numTokens = parent.numTokens;
            @display("p=577,84");
        }
    connections allowunconnected:
        // in --> classifier
        in --> { @display("m=w"); } --> classifier.in;
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
        // shaper --> out
        shaper.out --> { @display("m=e"); } --> out;
}
