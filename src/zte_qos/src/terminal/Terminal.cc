#include "packet/MetaTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "Terminal.h"

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

inet::Packet* genPacket(int pid, int tos, int src, int dst, int numBytes, double ts) {
    std::string title = "p_" + std::to_string(pid) + "_" + std::to_string(src) + "~" + std::to_string(dst) + "_tos=" + std::to_string(tos);
    auto data = inet::makeShared<inet::ByteCountChunk>(inet::B(numBytes));
    auto packet = new inet::Packet(title.c_str(), data);
    auto metaTag = packet->addTag<inet::MetaTag>();
    metaTag->setPid(pid);
    metaTag->setTos(tos);
    metaTag->setSrc(src);
    metaTag->setDst(dst);
    metaTag->setNumBytes(numBytes);
    metaTag->setTs(ts);
    return packet;
}

Define_Module(Terminal);

void Terminal::scheduleSendToSwitch(inet::Packet *pkt) {
    auto tag = pkt->getTag<inet::MetaTag>();
    std::string switchName = "s_" + std::to_string(tag->getSrc());
    auto targetGate = getSimulation()->getSystemModule()->getSubmodule(switchName.c_str())->gate("tIn");
    sendDirect(pkt, 0, tag->getTs(), targetGate);
}

void Terminal::initialize() {
    // self-message to sendDirect in handleMessage
    genMsg = new cMessage("GEN");
    scheduleAt(0, genMsg);
}

void Terminal::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg == genMsg) {
        // sendDirect all packets with duration=xx and propagationDelay=0 (unit = second).
        EV << "Terminal: generating packets." << "\n";
        scheduleSendToSwitch(genPacket(666666, 0, 0, 1, 2, 0.5));
    } else {
        error("unsupported message for terminal.");
    }
}

void Terminal::finish() {
    delete genMsg;
}

} // namespace terminal
} // namespace zte_qos
