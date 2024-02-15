#include <fstream>
#include <sstream>
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

void Terminal::generateTrace() {
    std::ifstream trace;
    trace.open(traceFile);
    if (trace.is_open()) {
        std::string line;
        std::string item;
        int pid;
        int tos;
        int src;
        int dst;
        int numBytes;
        double ts;
        while (std::getline(trace, line)) {
            // header line: skip
            if (line[0] == '#') {
                continue;
            }
            // decode line
            std::istringstream iss(line);
            std::getline(iss, item, ' ');   // id
            pid = std::stoi(item);
            std::getline(iss, item, ' ');   // tos
            tos = std::stoi(item);
            std::getline(iss, item, ' ');   // src
            src = std::stoi(item);
            std::getline(iss, item, ' ');   // dst
            dst = std::stoi(item);
            std::getline(iss, item, ' ');   // size
            numBytes = std::stoi(item);
            std::getline(iss, item, ' ');   // timestamp
            ts = std::stod(item);
            // construct and send packet
            scheduleSendToSwitch(genPacket(pid, tos, src, dst, numBytes, ts));
        }
    }
}

void Terminal::initialize() {
    // self-message to sendDirect in handleMessage
    genMsg = new cMessage("T_GEN");
    scheduleAt(0, genMsg);
}

void Terminal::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg == genMsg) {
        generateTrace();
    } else {
        error("unsupported message for terminal.");
    }
}

void Terminal::finish() {
    delete genMsg;
}

} // namespace terminal
} // namespace zte_qos
