#include "terminal.h"

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

Define_Module(Terminal);

void Terminal::initialize() {
    // send a ping message if she is the sender
    if (strcmp("sender", getName()) == 0) {
        cMessage *msg = new cMessage("ping");
        send(msg, "out");
    }
}

void Terminal::handleMessage(cMessage *msg) {
    // reply if she is the receiver
    if (strcmp("receiver", getName()) == 0) {
        cMessage *msg = new cMessage("pong");
        send(msg, "out");
    }
}

} // namespace terminal
} // namespace zte_qos
