#ifndef DATA_H
#define DATA_H

struct data_collection {
    float temperature_max;
    float temperature_min;
    float temperature_scheduled;
    float temperature_moment;       //water temperature
    
    float sun_power_moment;
    float sun_voltage;
    float sun_current;
    float pwm_duty;
    
    float device_temperature;
    float capacitor_temperature;

    bool sun_relay_on;
    bool grid_relay_on;
    bool transistor_overheat_on;
    bool pv_available;
    
    uint8_t current_mode;
};

namespace data_tests {
    void set_stats(float, float, float, float, float, float, float, bool, bool, bool);
    
    void set_temperature_max(float);
    void set_temperature_min(float);
    void set_temperature_scheduled(float);
    void set_temperature_moment(float);
    void set_sun_power_moment(float);
    void set_sun_voltage(float);
    void set_sun_current(float);
    void set_pwm_duty(float);
    void set_device_temperature(float);
    void set_capacitor_temperature(float);
    void set_sun_relay_on(bool);
    void set_grid_relay_on(bool);
    void set_transistor_overheat_on(bool);
    void set_pv_available(bool);
    void set_current_mode(uint8_t);
    
    float get_temperature_max(void);
    float get_temperature_min(void);
    float get_temperature_scheduled(void);
    float get_temperature_moment(void);
    float get_sun_power_moment(void);
    float get_sun_voltage(void);
    float get_sun_current(void);
    float get_pwm_duty(void);
    float get_device_temperature(void);
    float get_capacitor_temperature(void);
    bool get_sun_relay_on(void);
    bool get_grid_relay_on(void);
    bool get_transistor_overheat_on(void);
    bool get_pv_available(void);
    uint8_t get_current_mode(void);
}

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************
