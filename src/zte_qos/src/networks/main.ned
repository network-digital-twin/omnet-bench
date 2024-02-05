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
import inet.queueing.classifier.SingleRateThreeColorClassifier;
import inet.queueing.filter.RedDropper;
import inet.queueing.common.PacketMultiplexer;
import inet.queueing.sink.PassivePacketSink;
import inet.queueing.queue.PacketQueue;
import inet.queueing.server.TokenBasedServer;
import inet.queueing.tokengenerator.TimeBasedTokenGenerator;

// DummyLine is a dummy connection with a static datarate and transmission delay set, for testing.
channel DummyLine extends ned.DatarateChannel
{
    datarate = 16bps;
}

// ZteNet is the main benchmark network.
network ZteNet
{
    parameters:
        double redWq = default(0.002);
        @display("bgb=820,490");
    submodules:
        sender: Terminal {
            @display("p=106,232");
        }
        srTcm: SingleRateThreeColorClassifier {
            bitsPerToken = 1;
            @display("p=229,232");
        }
        redG: RedDropper {
            collectionModule = "^.queue";
            wq = parent.redWq;
            minth = 50;
            maxth = 100;
            maxp = 0.3;
            @display("p=355,230");
        }
        redY: RedDropper {
            collectionModule = "^.queue";
            wq = parent.redWq;
            minth = 30;
            maxth = 60;
            maxp = 0.6;
            @display("p=355,303");
        }
        mux: PacketMultiplexer {
            @display("p=433,232");
        }
        sink: PassivePacketSink {
            @display("p=355,155");
        }
        queue: PacketQueue {
            @display("p=499,232");
        }
        shaper: TokenBasedServer {
            @display("p=596,232");
        }
        tokenGen: TimeBasedTokenGenerator {
            storageModule = "^.shaper";
            @display("p=596,62");
        }
        receiver: PassivePacketSink {
            @display("p=697,232");
        }
    connections allowunconnected:
        // sender -> meter
        sender.out --> DummyLine --> srTcm.in;
        // meter -> dropper
        srTcm.out++ --> DummyLine { @display("ls=green"); } --> redG.in;
        srTcm.out++ --> DummyLine { @display("ls=yellow"); } --> redY.in;
        srTcm.out++ --> DummyLine { @display("ls=red"); } --> sink.in;
        // dropper --mux--> queue
        redG.out --> DummyLine --> mux.in++;
        redY.out --> DummyLine --> mux.in++;
        mux.out --> DummyLine --> queue.in;
        // queue --> shaper
        queue.out --> DummyLine --> shaper.in;
        // shaper --> receiver
        shaper.out --> DummyLine --> receiver.in;
}
