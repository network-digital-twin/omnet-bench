#include "packet/MetaTag_m.h"
#include "pipeline/classifier/Tos2QosClassifier.h"

namespace zte_qos {
namespace pipeline {
namespace classifier {

int Tos2QosClassifier::classifyPacket(inet::Packet *packet) const {
    auto tosTag = packet->getTag<inet::MetaTag>();
    int tos = tosTag->getTos();
    if (tos >= 0 && tos <= 2) {
        return tos;
    }
    return -1;
}

Register_Class(Tos2QosClassifier);

} // namespace classifier
} // namespace pipeline
} // namespace zte_qos
