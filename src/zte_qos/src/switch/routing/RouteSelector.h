#ifndef __ZTE_SWITCH_ROUTESEL_H
#define __ZTE_SWITCH_ROUTESEL_H

#include <omnetpp.h>
#include <map>
#include <vector>
#include "inet/queueing/base/PacketClassifierBase.h"

namespace zte_qos {
namespace switch_dvc {
namespace routing {

class RouteSelector: public inet::queueing::PacketClassifierBase {
protected:
    int sid;                // switch id
    std::string infoFile;   // location of the info.yaml file for the switch
    std::map<int, std::vector<int>> rt;  // routing table: dst switch id => [output gate id]
protected:
    virtual void initialize(int stage) override;
    virtual int classifyPacket(inet::Packet *packet) override;

    /***
     * loadRoutingInfo reads the routing specifications from the ".yaml" `infoFile` and
     * returns a map of dst switch ID => vector of outgoing port IDs of the current switch.
     */
    std::map<int, std::vector<std::string>> loadRoutingInfo();

    /***
     * getTranslationMap iterates through all output gates to find the corresponding outgoing port IDs of the
     * current switch from the links.
     */
    std::map<std::string, int> getTranslationMap();

    /***
     * initRoutingTable initializes `rt` using `infoFile` and extra information of the outgoing links
     * from the current switch. First, it loads the routing info map from the specified local YAML file.
     * Then, it iterates through all output gates to find the corresponding outgoing port IDs specified in
     * the links. Finally, it translates the routing info map to the desired routing table.
     */
    void initRoutingTable();

    /***
     * selectGate chooses a candidate gate id from the provided gates.
     */
    int selectGate(const std::vector<int> gateIds);
};

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos

#endif
