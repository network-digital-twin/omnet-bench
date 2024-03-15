#ifndef __ZTE_PIPELINE_DROPPER_RED_H
#define __ZTE_PIPELINE_DROPPER_RED_H

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"
#include "inet/queueing/filter/RedDropper.h"

namespace zte_qos {
namespace pipeline {
namespace dropper {

class RedDropper: public inet::queueing::RedDropper {
protected:
    virtual void dropPacket(inet::Packet *packet) override;
};

} // namespace dropper
} // namespace pipeline
} // namespace zte_qos

#endif
