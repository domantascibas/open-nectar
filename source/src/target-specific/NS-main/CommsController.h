#ifndef _NS_COMMS_CONTROLLER_H
#define _NS_COMMS_CONTROLLER_H

#include "mbed.h"

struct CommsController {
    CommsController();
    void sendPowerMessage();
    void sendEspMessage();
    void clearQueue();
    void freeChannel();
    void busyChannel();
    bool isChannelFree();

private:
    bool isFree;
    bool nextPower;
    bool nextEsp;
};

extern CommsController commsController;

#endif
