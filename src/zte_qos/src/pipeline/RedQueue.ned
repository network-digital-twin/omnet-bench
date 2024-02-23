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

import zte_qos.sink.Sink;
import inet.queueing.classifier.SingleRateThreeColorClassifier;
import inet.queueing.filter.RedDropper;
import inet.queueing.common.PacketMultiplexer;
import inet.queueing.queue.PacketQueue;

module RedQueue
{
    parameters:
        double wq = default(0.002);
        // redG
        double minthG = default(50);
        double maxthG = default(100);
        double maxpG = default(0.3);
        // redY
        double minthY = default(30);
        double maxthY = default(60);
        double maxpY = default(0.6);
        @display("i=block/boundedqueue;bgb=680,371");
    gates:
        input in;
        output out;
    submodules:
        srTcm: SingleRateThreeColorClassifier {
            bitsPerToken = 1;
            @display("p=94,175");
        }
        redG: RedDropper {
            collectionModule = "^.queue";
            wq = parent.wq;
            minth = parent.minthG;
            maxth = parent.maxthG;
            maxp = parent.maxpG;
            @display("p=305,175");
        }
        redY: RedDropper {
            collectionModule = "^.queue";
            wq = parent.wq;
            minth = parent.minthY;
            maxth = parent.maxthY;
            maxp = parent.maxpY;
            @display("p=305,248");
        }
        mux: PacketMultiplexer {
            @display("p=436,175");
        }
        dropSink: Sink {
            sinkType = "drop";
            @display("p=305,100");
        }
        queue: PacketQueue {
            @display("p=589,175");
        }
    connections allowunconnected:
        // in -> meter
        in --> { @display("m=w"); } --> srTcm.in;
        // meter -> dropper
        srTcm.out++ --> { @display("ls=green"); } --> redG.in;
        srTcm.out++ --> { @display("ls=yellow"); } --> redY.in;
        srTcm.out++ --> { @display("ls=red"); } --> dropSink.in;
        // dropper --mux--> queue
        redG.out --> mux.in++;
        redY.out --> mux.in++;
        mux.out --> queue.in;
        // queue --> out
        queue.out --> { @display("m=e"); } --> out;
}
