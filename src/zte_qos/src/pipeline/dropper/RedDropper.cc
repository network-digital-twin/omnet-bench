#include "pipeline/dropper/RedDropper.h"
#include "packet/MetaTag_m.h"
#include "util/json.h"
using json = nlohmann::json;

namespace zte_qos {
namespace pipeline {
namespace dropper {

Define_Module(RedDropper);

void RedDropper::dropPacket(inet::Packet *packet) {
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
                    {"drop", 1}
            }}
    };
    EV_WARN << "[RES] " << res.dump() << std::endl;
    inet::queueing::RedDropper::dropPacket(packet);
}

} // namespace dropper
} // namespace pipeline
} // namespace zte_qos
