#include "mbed.h"
#include "device_modes.h"
#include "data.h"
#include <string>
#include "service.h"

static const PinName PC_TX = PA_2;
static const PinName PC_RX = PA_3;

RawSerial pc(PC_TX, PC_RX);

namespace service {
  void setup() {
    static const uint32_t pc_baud = 115200;
    
    pc.baud(pc_baud);
    printf("\r\n[START]\r\n");
    printf("[COMMS PC]\r\n");
    printf("[ok] baud %d\r\n\n", pc_baud);
  }
}