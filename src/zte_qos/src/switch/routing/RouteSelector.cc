#include "packet/MetaTag_m.h"
#include "switch/routing/RouteSelector.h"

namespace zte_qos {
namespace switch_dvc {
namespace routing {

Define_Module(RouteSelector);

void RouteSelector::initialize(int stage) {
    PacketClassifierBase::initialize(stage);
    if (stage == 0) {
        sid = par("sid");
    }
}

int RouteSelector::classifyPacket(inet::Packet *packet) {
    auto tag = packet->getTag<inet::MetaTag>();
    if (tag->getDst() == sid) {
        // this is the destination, end here -> index N
        return outputGates.size() - 1;
    }
    return 0;
}

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos
