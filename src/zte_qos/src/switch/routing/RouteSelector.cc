#include <fstream>
#include "packet/MetaTag_m.h"
#include "switch/routing/RouteSelector.h"
#include "util/json.h"
using json = nlohmann::json;

namespace zte_qos {
namespace switch_dvc {
namespace routing {

Define_Module(RouteSelector);

std::map<int, std::vector<std::string>> RouteSelector::loadRoutingInfo() {
    std::map<int, std::vector<std::string>> res;
    std::string infoFileExt = infoFile.substr(infoFile.rfind(".") + 1, infoFile.size() - 1);
    if (std::strcmp(infoFileExt.c_str(), "yaml") == 0) {
        // TODO
        EV << "YAML: to be implemented" << std::endl;
    } else if (std::strcmp(infoFileExt.c_str(), "json") == 0) {
        std::ifstream f(infoFile);
        json routing = json::parse(f)["routing"];
        f.close();
        for (json::iterator it = routing.begin(); it != routing.end(); ++it) {
            std::string dstStr = it.key();
            std::string port = it.value()["port"];
            res[std::stoi(dstStr)] = std::vector<std::string>{port};
        }
    } else {
        error("Unsupported infoFile format: %s", infoFileExt.c_str());
    }
    return res;
}

std::map<std::string, int> RouteSelector::getTranslationMap() {
    std::map<std::string, int> transMap;
    auto curSwitch = getParentModule();
    const char *outGateName = "out";
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
