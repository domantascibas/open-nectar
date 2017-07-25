#ifndef DATA_H
#define DATA_H

enum codes {
    NS_OK,
    NS_ERROR,
    MSG_OK,
    MSG_ERROR
};

enum mode_power_source {
    TURN_ON_GRID,
    TURN_ON_PV,
    TURN_OFF_ALL
};

enum mode_operational {
    MODE_DEFAULT,
    MODE_BOOST,
    MODE_AWAY,
    MODE_NO_GRID
};

struct Data {
    uint8_t current_mode;
    
    float temp_max;
    float temp_min;
    float temp_scheduled;
    float temp_boiler;
    
    bool  sun_relay_on;
    bool  grid_relay_on;
    
    float pv_power;
    float pv_voltage;
    float pv_current;
    float pv_ref_voltage;
    float pv_ref_current;
    bool  pv_available;
    
    float pwm_duty;
    float capacitor_temp;
    float device_temperature;
    
    uint8_t  mosfet_overheat_on;
};

extern Data data;

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************
