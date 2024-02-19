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

package zte_qos.switch;

import zte_qos.switch.routing.RouteSelector;
import zte_qos.pipeline.QosPipeline;
import inet.queueing.common.PacketMultiplexer;
import inet.queueing.sink.PassivePacketSink;

module Switch
{
    parameters:
        int sid = default(-1);
        string infoFile;
        @display("i=device/accesspoint");
    gates:
        input tIn;	// entrance for terminal
        input in[];
        output out[];
    submodules:
        mux: PacketMultiplexer {
            @display("p=85.36,79.2");
        }
        routeSel: RouteSelector {
            sid = parent.sid;
            infoFile = parent.infoFile;
            @display("p=186.56,78.32");
        }
        dstSink: PassivePacketSink {
            @display("p=186,175");
        }
        qos[sizeof(out)]: QosPipeline {
            @display("p=330.88,77.44,c,100");
        }
    connections allowunconnected:
        // in --> routeSel
        tIn --> { @display("m=n;t=tIn"); } --> mux.in++;
        for i=0..sizeof(in)-1 {
            in[i] --> { @display("m=w"); } --> mux.in++;
        }
        mux.out --> routeSel.in;
        // routeSel --> qos --> out
        for i=0..sizeof(out)-1 {
            routeSel.out++ --> qos[i].in;
            qos[i].out --> { @display("m=e"); } --> out[i];
        }
        // routeSel --> sink (as dst, index N-1)
        routeSel.out++ --> dstSink.in;
}
