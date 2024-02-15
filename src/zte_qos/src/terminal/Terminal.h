#ifndef __ZTE_TERMINAL_H
#define __ZTE_TERMINAL_H

#include <omnetpp.h>

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

/***
 * genPacket generates a packet according to the given information.
 */
inet::Packet* genPacket(int pid, int tos, int src, int dst, int numBytes, double ts);

/***
 * Terminal generates packets and sends them to the corresponding switches.
 */
class Terminal: public cSimpleModule {
protected:
    cMessage *genMsg;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    /***
     * scheduleSendToSwitch schedules to send the packet to the source switch at the specified timestamp.
     * Relevant information should have been included in the MetaTag of the packet.
     */
    void scheduleSendToSwitch(inet::Packet *pkt);
};

} // namespace terminal
} // namespace zte_qos

#endif
