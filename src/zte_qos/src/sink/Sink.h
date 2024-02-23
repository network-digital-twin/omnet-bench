#ifndef __ZTE_SINK_H
#define __ZTE_SINK_H

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"
#include "inet/queueing/sink/PassivePacketSink.h"

namespace zte_qos {
namespace sink {

class Sink: public inet::queueing::PassivePacketSink {
protected:
    std::string sinkType;
protected:
    virtual void initialize(int stage) override;
    virtual void consumePacket(inet::Packet *packet) override;
};

} // namespace sink
} // namespace zte_qos

#endif
