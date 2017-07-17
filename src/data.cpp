#include "mbed.h"
#include "data.h"

extern data_collection nectar_data;

namespace data_tests {
    void set_stats(float temp_moment, float power, float voltage, float current, float duty, float device_temp, float capacitor_temp, bool sun_relay_on, bool grid_relay_on, bool transistor_overheat) {
        nectar_data.temperature_moment = temp_moment;
        nectar_data.sun_power_moment = power;
        nectar_data.sun_voltage = voltage;
        nectar_data.sun_current = current;
        nectar_data.pwm_duty = duty;
        nectar_data.device_temperature = device_temp;
        nectar_data.capacitor_temperature = capacitor_temp;
        nectar_data.sun_relay_on = sun_relay_on;
        nectar_data.grid_relay_on = grid_relay_on;
        nectar_data.transistor_overheat_on = transistor_overheat;
    }
    
    void set_temperature_max(float temp) {
        nectar_data.temperature_max = temp;
    }
    
    void set_temperature_min(float temp) {
        nectar_data.temperature_min = temp;
    }
    
    void set_temperature_scheduled(float temp) {
        nectar_data.temperature_scheduled = temp;
    }
    
    void set_temperature_moment(float temp) {
        nectar_data.temperature_moment = temp;
    }
    
    void set_sun_power_moment(float power) {
        nectar_data.sun_power_moment = power;
    }
    
    void set_sun_voltage(float voltage) {
        nectar_data.sun_voltage = voltage;
    }
    
    void set_sun_current(float current) {
        nectar_data.sun_current = current;
    }
    
    void set_pwm_duty(float duty) {
        nectar_data.pwm_duty = duty;
    }
    
    void set_device_temperature(float temp) {
        nectar_data.device_temperature = temp;
    }
    
    void set_capacitor_temperature(float temp) {
        nectar_data.capacitor_temperature = temp;
    }
    
    void set_sun_relay_on(bool state) {
        nectar_data.sun_relay_on = state;
    }
    
    void set_grid_relay_on(bool state) {
        nectar_data.grid_relay_on = state;
    }
    
    void set_transistor_overheat_on(bool state) {
        nectar_data.transistor_overheat_on = state;
    }
    
    void set_pv_available(bool state) {
        nectar_data.pv_available = state;
    }
    
    void set_current_mode(uint8_t mode) {
        nectar_data.current_mode = mode;
    }
    
    float get_temperature_moment() {
        return nectar_data.temperature_moment;
    }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
