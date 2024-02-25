#include "sink/Sink.h"
#include "packet/MetaTag_m.h"
#include "util/json.h"
using json = nlohmann::json;

namespace zte_qos {
namespace sink {

Define_Module(Sink);

void Sink::initialize(int stage) {
    PassivePacketSink::initialize(stage);
    if (stage == 0) {
        sinkType = par("sinkType").stringValue();
    }
}

void Sink::consumePacket(inet::Packet *packet) {
    auto tag = packet->getTag<inet::MetaTag>();
    json res = {
            {"type", "packet"},
            {"module", getFullPath()},
            {"metrics", {
                    {"pid", tag->getPid()},
                    {"src", tag->getSrc()},
                    {"dst", tag->getDst()},
                    {"start_ts", tag->getTs()},
                    {"end_ts", omnetpp::simTime().dbl() * 1000000000},
                    {"drop", std::strcmp(sinkType.c_str(), "drop") == 0 ? 1 : 0}
            }}
    };
    EV_WARN << "[RES] " << res.dump() << std::endl;
    PassivePacketSink::consumePacket(packet);
}

} // namespace sink
} // namespace zte_qos
