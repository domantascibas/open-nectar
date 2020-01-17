#ifndef _NS_SUN_RELAY_H
#define _NS_SUN_RELAY_H

#include "consts.h"
#include "pins.h"
#include "Relay.h"

struct SunRelay : Relay {
    SunRelay() : Relay(SUN_RELAY_PIN) {};
    void init();
    virtual void turnOn();
    virtual void turnOff();

private:
    void timeoutIsr();
    void timeoutSd();
};

#endif
