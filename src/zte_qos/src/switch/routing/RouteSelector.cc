#include "packet/MetaTag_m.h"
#include "switch/routing/RouteSelector.h"

namespace zte_qos {
namespace switch_dvc {
namespace routing {

Define_Module(RouteSelector);

std::map<int, std::vector<std::string>> RouteSelector::loadRoutingInfo() {
    // TODO: read `infoFile` and get a map: dstId => [portId]
    // Can switch on .yaml file and .json file, and implement json first for now
    // do sth here.

    // TODO: PoC, remove later
    std::map<int, std::vector<std::string>> testInfo;
    if (strcmp(getParentModule()->getName(), "s_0") == 0) {
        testInfo[1] = std::vector<std::string>{"port_0_1__0"};
        testInfo[2] = std::vector<std::string>{"port_0_2__0"};
    }
    return testInfo;
}

std::map<std::string, int> RouteSelector::getTranslationMap() {
    std::map<std::string, int> transMap;
    auto curSwitch = getParentModule();
    const char *outGateName = "out";    // TODO: read from global const spec files
    for (int gateId = 0; gateId < curSwitch->gateSize(outGateName); ++gateId) {
        auto curLink = curSwitch->gate(outGateName, gateId)->getNextGate()->getOwnerModule();
        auto curPortId = curLink->getProperties()->get("metadata")->getValue("srcPort");
        transMap[curPortId] = gateId;
    }
    return transMap;
}

void RouteSelector::initRoutingTable() {
    auto infoMap = loadRoutingInfo();
    auto transMap = getTranslationMap();
    for (auto infoIt = infoMap.begin(); infoIt != infoMap.end(); ++infoIt) {
        std::vector<int> curToGates;
        for (auto p: infoIt->second) {
            curToGates.push_back(transMap[p]);
        }
        rt[infoIt->first] = curToGates;
    }
}

int RouteSelector::selectGate(const std::vector<int> gateIds) {
    // (current impl: just select the first gate)
    return gateIds[0];
}

void RouteSelector::initialize(int stage) {
    PacketClassifierBase::initialize(stage);
    if (stage == 0) {
        sid = par("sid");
        infoFile = par("infoFile").stringValue();
        initRoutingTable();
    }
}

int RouteSelector::classifyPacket(inet::Packet *packet) {
    auto tag = packet->getTag<inet::MetaTag>();
    if (tag->getDst() == sid) {
        // this is the destination, end here -> index N (NOTE: `outputGates` here is for RouteSelector, not switch)
        return outputGates.size() - 1;
    }
    if (rt.find(tag->getDst()) != rt.end()) {
        // find the route -> corresponding [gate id] in the routing table
        auto gateIds = rt[tag->getDst()];
        // select a candidate gate id as the result
        auto selectedGateId = selectGate(gateIds);
        return selectedGateId;
    }
    return -1;
}

} // namespace routing
} // namespace switch_dvc
} // namespace zte_qos
