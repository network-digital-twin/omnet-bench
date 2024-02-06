#ifndef __ZTE_TERMINAL_H
#define __ZTE_TERMINAL_H

#include <omnetpp.h>

using namespace omnetpp;

namespace zte_qos {
namespace terminal {

/***
 * Terminal generates packets and sends them to the corresponding switches.
 */
class Terminal: public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

} // namespace terminal
} // namespace zte_qos

#endif
