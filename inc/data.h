#ifndef DATA_H
#define DATA_H

struct data_collection {
    float temperature_max;
    float temperature_min;
    float temperature_scheduled;
    float temperature_moment;       //water temperature
    
    float moment_sun_watt;
    float sun_voltage;
    float sun_current;
    float pwm_duty;
    
    float device_temperature;
    float capacitor_tempearature;

    bool sun_relay_on;
    bool grid_relay_on;
    bool transistor_overheat_on;
    
    uint8_t current_mode;
} nectar_data;

namespace data {
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************
