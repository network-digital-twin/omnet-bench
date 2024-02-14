#ifndef __ZTE_SWITCH_ROUTESEL_H
#define __ZTE_SWITCH_ROUTESEL_H

#include <omnetpp.h>
#include "inet/queueing/contract/IPacketClassifierFunction.h"

namespace zte_qos {
namespace switch_dvc {
namespace routing {

class RouteSelector : public omnetpp::cObject, public inet::queueing::IPacketClassifierFunction
{
public:
    virtual int classifyPacket(inet::Packet *packet) const override;
};

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos

#endif
