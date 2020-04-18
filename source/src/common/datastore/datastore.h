#ifndef _NS_DATASTORE_H
#define _NS_DATASTORE_H

#include <stdint.h>

typedef struct {
    // ---------------------------------------------------------------
    // Board data
    // ---------------------------------------------------------------
    struct {
        char ucVersion[12];                     // 12B

        struct {
            uint8_t ucProcessor;                // 1B
        } sTemperature;

        struct {
            uint16_t usVref;                    // 2B
            uint16_t usVss3V3;                  // 2B
            uint16_t usVss5V;                   // 2B
            uint16_t usVss12V;                  // 2B
        } sVoltage;
    } sBoard;

    // ---------------------------------------------------------------
    // Energy meter data
    // ---------------------------------------------------------------
    struct {
        uint32_t grid;                          // 4B
        uint32_t sun;                           // 4B
    } sEnergyMeters;

    // ---------------------------------------------------------------
    // Error data
    // ---------------------------------------------------------------
    struct {
        uint16_t usErrors;                      // 2B
    } sErrors;

    // ---------------------------------------------------------------
    // Device Settings data
    // ---------------------------------------------------------------
    struct {
        // current device mode
        // boost enabled
        // boiler power

        // max temp
        // target temp
        // night temp

        // wifi configured
        // power board calibrated
        // ref voltage
        // ref current
    } sSettings;


//     struct {
//         /*

// relay state
//   float sun_power;
//   float sun_voltage;
//   float sun_current;
//   float pwm_duty;

//   float device_temperature;
//   uint8_t transistor_overheat_on;
//   uint8_t device_calibrated;

//   uint8_t pwm_generator_on;
//   float sun_meter_kwh;
//   float grid_meter_kwh;
//  */
//     } s;

} datastore_t;

extern datastore_t datastore;

#endif
