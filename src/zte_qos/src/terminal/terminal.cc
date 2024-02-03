#include "terminal.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/BitCountChunk.h"

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

Define_Module(Terminal);

void Terminal::initialize() {
    // send a ping message if she is the sender
    if (strcmp("sender", getName()) == 0) {
        auto data = inet::makeShared<inet::BitCountChunk>(inet::b(100000000));
        auto packet = new inet::Packet("ping", data);
        send(packet, "out");
    }
}

void Terminal::handleMessage(cMessage *msg) {
    // reply if she is the receiver
    if (strcmp("receiver", getName()) == 0) {
        auto data = inet::makeShared<inet::BitCountChunk>(inet::b(100000000));
        auto packet = new inet::Packet("pong", data);
        send(packet, "out");
    }
}

} // namespace terminal
} // namespace zte_qos
