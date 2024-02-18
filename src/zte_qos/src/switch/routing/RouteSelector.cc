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
        if (strcmp(getParentModule()->getName(), "s_0") == 0) {
            rt[1] = 0;
            rt[2] = 1;
        }
    }
}

int RouteSelector::classifyPacket(inet::Packet *packet) {
    auto tag = packet->getTag<inet::MetaTag>();
    if (tag->getDst() == sid) {
        // this is the destination, end here -> index N
        return outputGates.size() - 1;
    }
    if (rt.find(tag->getDst()) != rt.end()) {
        // find the route -> corresponding gate id in the routing table
        return rt[tag->getDst()];
    }
    return -1;
}

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos
