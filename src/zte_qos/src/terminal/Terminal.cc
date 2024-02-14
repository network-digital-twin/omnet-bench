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
    // TODO: self-message to sendDirect in handleMessage
    // send a ping packet if she is the sender
    if (strcmp("sender", getName()) == 0) {
        // create a packet of size=100Mb and tos=1 as a packet tag
        int tos = 1;
        int numBytes = 2;
        std::string title = "ping_tos_" + std::to_string(tos);
        auto data = inet::makeShared<inet::ByteCountChunk>(inet::B(numBytes));
        auto packet = new inet::Packet(title.c_str(), data);
        auto metaTag = packet->addTag<inet::MetaTag>();
        metaTag->setTos(tos);
        metaTag->setSrc(12);
        metaTag->setDst(21);
        metaTag->setNumBytes(numBytes);
        send(packet, "out");
    }
}

void Terminal::handleMessage(cMessage *msg) {
    // TODO: if self-message, sendDirect all packets with duration=xxs and propagationDelay=0.
    // reply if she is the receiver
    if (strcmp("receiver", getName()) == 0) {
        auto recvPkt = check_and_cast<inet::Packet*>(msg);
        auto recvTag = recvPkt->getTag<inet::MetaTag>();
        int tos = recvTag->getTos() * 100;
        int numBytes = recvTag->getNumBytes() * 2;  // 4
        std::string title = "pong_tos_" + std::to_string(tos);
        auto data = inet::makeShared<inet::ByteCountChunk>(inet::B(numBytes));
        auto packet = new inet::Packet(title.c_str(), data);
        auto newTag = packet->addTag<inet::MetaTag>();
        newTag->setTos(tos);
        newTag->setSrc(recvTag->getSrc() * 3);  // 36
        newTag->setDst(recvTag->getDst() * 4);  // 84
        newTag->setNumBytes(numBytes);
        sendDelayed(packet, 3, "out");
    }
}

} // namespace terminal
} // namespace zte_qos
