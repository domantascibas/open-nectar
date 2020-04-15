#include "consts.h"
#include "pins.h"
#include "u8x8.h"

I2C i2c(SCREEN_SDA_PIN, SCREEN_SCL_PIN);

uint8_t u8g2_byte_hw_i2c_mbed(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    uint8_t *data;

    switch (msg) {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;

            while (arg_int > 0) {
                i2c.write(*data);
                data++;
                arg_int--;
            }

            break;

        case U8X8_MSG_BYTE_INIT:
            i2c.frequency(SCREEN_I2C_FREQUENCY);
            break;
        case U8X8_MSG_BYTE_SET_DC:
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            i2c.start();
            i2c.write(u8x8_GetI2CAddress(u8x8));
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            i2c.stop();
            break;
        default:
            return 0;
    }
    return 1;
}

uint8_t u8g2_gpio_and_delay_mbed(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    return 1;
}
