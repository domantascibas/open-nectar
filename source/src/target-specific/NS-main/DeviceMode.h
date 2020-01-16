#ifndef _NS_DEVICE_MODE_H
#define _NS_DEVICE_MODE_H

#include <stdint.h>

typedef enum {
    WELCOME,
    NOT_CONFIGURED,
    CONFIGURED,
    TEST_MODE
} opModes;

void deviceOpMode_init(void);
uint8_t deviceOpMode_getCurrentMode(void);
void deviceOpMode_setConfigured(void);
void deviceOpMode_resetConfiguration(void);
void deviceOpMode_setTestMode(void);
void deviceOpMode_endOnboarding(void);
void deviceOpMode_setInTestStand(void);
uint8_t deviceOpMode_isInTestMode(void);
uint8_t deviceOpMode_isOnboarding(void);
uint8_t deviceOpMode_isConfigured(void);
uint8_t deviceOpMode_isPairing(void);
uint8_t deviceOpMode_isReset(void);
uint8_t deviceOpMode_isBoostOff(void);
uint8_t deviceOpMode_isInTestStand(void);
uint8_t deviceOpMode_isLoading(void);
void deviceOpMode_endLoading(void);
void deviceOpMode_setPairing(uint8_t input);
void deviceOpMode_setReset(uint8_t input);
void deviceOpMode_setBoostOff(uint8_t input);

#endif
