#ifndef __ZTE_SWITCH_ROUTESEL_H
#define __ZTE_SWITCH_ROUTESEL_H

#include <omnetpp.h>
#include <map>
#include "inet/queueing/base/PacketClassifierBase.h"

namespace zte_qos {
namespace switch_dvc {
namespace routing {

class RouteSelector: public inet::queueing::PacketClassifierBase {
protected:
    int sid;                // switch id
    std::map<int, int> rt;  // routing table: dst switch id => output gate id (TODO: maybe `map<int, map<string, int>>` for dst + port => gate)
protected:
    virtual void initialize(int stage) override;
    virtual int classifyPacket(inet::Packet *packet) override;
};

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos

#endif
