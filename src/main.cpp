#include "mbed.h"
#include "hardware.h"
#include "power_board.h"
#include "esp.h"
#include "modes.h"

static const PinName TX = PA_2;
static const PinName RX = PA_3;

RawSerial pc(TX, RX);

void setup() {
    pc.baud(115200);
    pc.printf("[COMMS] Started\r\n");
}

int main() {
    setup();
    hardware::setup();
    wait(2.0);
    power_board::setup();
    esp::setup();
    
    while(1) {
        power_board::loop();
        esp::loop();
        nectar::loop();
        hardware::loop();
        __WFI();
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
