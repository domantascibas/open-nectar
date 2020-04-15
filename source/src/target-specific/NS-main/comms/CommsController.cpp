#include "CommsController.h"
#include "EspComms.h"
#include "PowerBoardComms.h"

CommsController commsController;

CommsController::CommsController() {
    isFree = true;
    nextPower = false;
    nextEsp = false;
}

void CommsController::sendPowerMessage() {
    if (isFree) {
        isFree = false;
        nextPower = false;
        power_board::send_message();
    } else {
        if (!nextPower) {
            nextPower = true;
        }
    }
}

void CommsController::sendEspMessage() {
    if (isFree) {
        isFree = false;
        nextEsp = false;
        esp::send_message();
    } else {
        if (!nextEsp) {
            nextEsp = true;
        }
    }
}

void CommsController::clearQueue() {
    if (nextEsp) {
        isFree = false;
        nextEsp = false;
        esp::send_message();
    }
    if (nextPower) {
        isFree = false;
        nextPower = false;
        power_board::send_message();
    }
}

void CommsController::busyChannel() {
    isFree = false;
}

void CommsController::freeChannel() {
    isFree = true;
}

bool CommsController::isChannelFree() {
    return isFree;
}
