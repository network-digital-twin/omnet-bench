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

// PingPong is a Terminal - Terminal ping pong testing network.
network PingPong
{
    @display("bgb=453,393");
    submodules:
        sender: Terminal {
            @display("p=80,194");
        }
        receiver: Terminal {
            @display("p=372,194");
        }
    connections allowunconnected:
        sender.out --> {  datarate = 8bps; delay = 100us; } --> receiver.in;
        receiver.out --> {  datarate = 8bps; delay = 50us; } --> sender.in;
}
