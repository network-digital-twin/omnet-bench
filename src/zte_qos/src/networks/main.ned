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
import inet.queueing.common.PacketDelayer;
import inet.queueing.classifier.SingleRateThreeColorClassifier;
import inet.queueing.filter.RedDropper;
import inet.queueing.common.PacketMultiplexer;
import inet.queueing.sink.PassivePacketSink;
import inet.queueing.queue.PacketQueue;
import inet.queueing.server.TokenBasedServer;
import inet.queueing.tokengenerator.TimeBasedTokenGenerator;

// ZteNet is the main benchmark network.
network ZteNet
{
    parameters:
        double redWq = default(0.002);
        @display("bgb=1277,490");
    submodules:
        sender: Terminal {
            @display("p=56,232");
        }
        inChan: PacketDelayer {
            @display("p=178,232");
        }
        srTcm: SingleRateThreeColorClassifier {
            bitsPerToken = 1;
            @display("p=300,232");
        }
        redG: RedDropper {
            collectionModule = "^.queue";
            wq = parent.redWq;
            minth = 50;
            maxth = 100;
            maxp = 0.3;
            @display("p=511,232");
        }
        redY: RedDropper {
            collectionModule = "^.queue";
            wq = parent.redWq;
            minth = 30;
            maxth = 60;
            maxp = 0.6;
            @display("p=511,305");
        }
        mux: PacketMultiplexer {
            @display("p=642,232");
        }
        sink: PassivePacketSink {
            @display("p=511,157");
        }
        queue: PacketQueue {
            @display("p=795,232");
        }
        shaper: TokenBasedServer {
            @display("p=952,232");
        }
        tokenGen: TimeBasedTokenGenerator {
            storageModule = "^.shaper";
            @display("p=952,62");
        }
        outChan: PacketDelayer {
            @display("p=1085,232");
        }
        receiver: PassivePacketSink {
            @display("p=1217,232");
        }
    connections allowunconnected:
        // sender -> meter
        sender.out --> inChan.in;
        inChan.out --> srTcm.in;
        // meter -> dropper
        srTcm.out++ --> { @display("ls=green"); } --> redG.in;
        srTcm.out++ --> { @display("ls=yellow"); } --> redY.in;
        srTcm.out++ --> { @display("ls=red"); } --> sink.in;
        // dropper --mux--> queue
        redG.out --> mux.in++;
        redY.out --> mux.in++;
        mux.out --> queue.in;
        // queue --> shaper
        queue.out --> shaper.in;
        // shaper --> receiver
        shaper.out --> outChan.in;
        outChan.out --> receiver.in;
}
