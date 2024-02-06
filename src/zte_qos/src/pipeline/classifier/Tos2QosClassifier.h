#ifndef __ZTE_PIPELINE_CLASSIFIER_H
#define __ZTE_PIPELINE_CLASSIFIER_H

#include <omnetpp.h>
#include "inet/queueing/contract/IPacketClassifierFunction.h"

namespace zte_qos {
namespace pipeline {
namespace classifier {

class Tos2QosClassifier : public omnetpp::cObject, public inet::queueing::IPacketClassifierFunction
{
public:
    virtual int classifyPacket(inet::Packet *packet) const override;
};

} // namespace classifier
} // namespace pipeline
} // namespace zte_qos

#endif
