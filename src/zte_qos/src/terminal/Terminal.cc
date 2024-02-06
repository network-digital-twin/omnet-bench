#include <iostream>
#include "packet/MetaTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "Terminal.h"

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

Define_Module(Terminal);

void Terminal::initialize() {
    // send a ping packet if she is the sender
    if (strcmp("sender", getName()) == 0) {
        // create a packet of size=100Mb and tos=1 as a packet tag
        int tos = 114514;
        std::string title = "ping_tos_" + std::to_string(tos);
        auto data = inet::makeShared<inet::ByteCountChunk>(inet::B(2));
        auto packet = new inet::Packet(title.c_str(), data);
        auto metaTag = packet->addTag<inet::MetaTag>();
        metaTag->setTos(tos);
        send(packet, "out");
    }
}

void Terminal::handleMessage(cMessage *msg) {
    // reply if she is the receiver
    if (strcmp("receiver", getName()) == 0) {
        auto recvPkt = check_and_cast<inet::Packet*>(msg);
        auto recvTag = recvPkt->getTag<inet::MetaTag>();
        int tos = recvTag->getTos() * 100;
        std::string title = "pong_tos_" + std::to_string(tos);
        auto data = inet::makeShared<inet::ByteCountChunk>(inet::B(2));
        auto packet = new inet::Packet(title.c_str(), data);
        send(packet, "out");
    }
}

} // namespace terminal
} // namespace zte_qos
