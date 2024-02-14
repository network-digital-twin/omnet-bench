#include "packet/MetaTag_m.h"
#include "switch/routing/RouteSelector.h"

namespace zte_qos {
namespace switch_dvc {
namespace routing {

int RouteSelector::classifyPacket(inet::Packet *packet) const {
    // TODO
    return 0;
}

Register_Class(RouteSelector);

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos
