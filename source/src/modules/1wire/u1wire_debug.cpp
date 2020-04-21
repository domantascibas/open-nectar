#include "mbed.h"
#include "u1wire.h"
#include <stdio.h>

#ifndef U1WIRE_DEBUG
#define U1WIRE_DEBUG 1
#endif

void u1wire_debug(u1wire_t dev)
{
#if U1WIRE_DEBUG
    uint8_t i;

    printf("%02x: ", dev->rom_code.fields.family);

    for (i = 0; i < sizeof(dev->rom_code.fields.serial); i++) {
        printf("%02x ", dev->rom_code.fields.serial[i]);
    }

    printf(":%02x\r\n", dev->rom_code.fields.crc);
#endif
}
