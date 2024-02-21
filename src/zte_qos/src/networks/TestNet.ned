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
import zte_qos.link.Link;

// TestNet is a small-scale network for testing module functionalities.
network TestNet
{
    parameters:
        string dataDir = "dataset/test/";
       	t.traceFile = dataDir + "trace.txt";
       	s_*.infoDir = dataDir + "info/";
       	s_*.infoFileType = "json";
    submodules:
        // terminal
        t: Terminal;
        // switches
        s_0: Switch {
            sid = 0;
            @display("p=61,88");
        }
        s_1: Switch {
            sid = 1;
            @display("p=256,58");
        }
        s_2: Switch {
            sid = 2;
            @display("p=255,142");
        }
        // links
        l_0_1__0: Link {
            bitrate = 32bps;
            delay = 10us;
            @display("p=152,57");
            @metadata(srcPort="port_0_1__0");
        }
        l_0_2__0: Link {
            bitrate = 32bps;
            delay = 10us;
            @display("p=149,142");
            @metadata(srcPort="port_0_2__0");
        }
    connections allowunconnected:
        // s_0 --> s_1
        s_0.out++ --> l_0_1__0.in;
        l_0_1__0.out --> s_1.in++;
        // s_0 --> s_1
        s_0.out++ --> l_0_2__0.in;
        l_0_2__0.out --> s_2.in++;
}
