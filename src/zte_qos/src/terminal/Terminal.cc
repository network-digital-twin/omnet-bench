#include <fstream>
#include <sstream>
#include "packet/MetaTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "Terminal.h"

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

inet::Packet* genPacket(int pid, int mid, int src, int dst, int numBytes, double ts, int tos) {
    std::string title = "p_" + std::to_string(pid) + "_" + std::to_string(src) + "~" + std::to_string(dst) + "_tos=" + std::to_string(tos);
    auto data = inet::makeShared<inet::ByteCountChunk>(inet::B(numBytes));
    auto packet = new inet::Packet(title.c_str(), data);
    auto metaTag = packet->addTag<inet::MetaTag>();
    metaTag->setPid(pid);
    metaTag->setMid(mid);
    metaTag->setSrc(src);
    metaTag->setDst(dst);
    metaTag->setNumBytes(numBytes);
    metaTag->setTs(ts);
    metaTag->setTos(tos);
    return packet;
}

Define_Module(Terminal);

void Terminal::scheduleSendToSwitch(inet::Packet *pkt) {
    auto tag = pkt->getTag<inet::MetaTag>();
    std::string switchName = "s_" + std::to_string(tag->getSrc());
    auto targetGate = getSimulation()->getSystemModule()->getSubmodule(switchName.c_str())->gate("tIn");
    sendDirect(pkt, 0, tag->getTs() / 1000000000.0, targetGate);
}

void Terminal::generateTrace() {
    std::ifstream trace;
    trace.open(traceFile);
    if (!trace.is_open()) {
        error("cannot open trace file: %s", traceFile.c_str());
    }
    std::string line;
    std::string item;
    int pid;
    int mid;
    int src;
    int dst;
    int numBytes;
    double ts;
    int tos;
    while (std::getline(trace, line)) {
        // header line: skip
        if (line[0] == '#') {
            continue;
        }
        // decode line
        std::istringstream iss(line);
        std::getline(iss, item, ' ');   // pid
        pid = std::stoi(item);
        std::getline(iss, item, ' ');   // mid
        mid = std::stoi(item);
        std::getline(iss, item, ' ');   // src
        src = std::stoi(item);
        std::getline(iss, item, ' ');   // dst
        dst = std::stoi(item);
        std::getline(iss, item, ' ');   // size (B)
        numBytes = std::stoi(item);
        std::getline(iss, item, ' ');   // timestamp (ns)
        ts = std::stod(item);
        std::getline(iss, item, ' ');   // tos
        tos = std::stoi(item);
        // construct and send packet
        scheduleSendToSwitch(genPacket(pid, mid, src, dst, numBytes, ts, tos));
    }
}

void Terminal::initialize() {
    genMsg = new cMessage("T_GEN");
    traceFile = par("traceFile").stringValue();
    // self-message to sendDirect in handleMessage
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
    EV_WARN << "[RES] wow..." << std::endl;
}

} // namespace terminal
} // namespace zte_qos
