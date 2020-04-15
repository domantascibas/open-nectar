#include "consts.h"
#include "pins.h"
#include "temperature_controller.h"
#include "processor_temperature.h"
#include "temperature_sensor.h"
// #include "error_controller.h"
#include "error_handler.h"
// #include "data.h"

extern "C" {
#include "data.h"
}

TemperatureSensor deviceTemp(INTERNAL_TEMPERATURE_PIN, 5);

void temperature_controller_init(void) {
    printf("Temperature controller setup\r\n");
    processor_temperature_init();
    deviceTemp.init();

    if (deviceTemp.isSensorFound()) {
        error_clear(NS_DEVICE_OVERHEAT);
    }
}

void temperature_controller_update_processor_temp(void) {
    float processor_temp = processor_temperature_measure();
    printf("processor temp: %f\r\n", internal_temp);
    if (processor_temp > PROCESSOR_INTERNAL_TEMPERATURE_LIMIT) {
        if (!error_isSet(NS_PROCESSOR_OVERHEAT_POWER)) {
            error_set(NS_PROCESSOR_OVERHEAT_POWER);
        }
        printf("PROCESSOR OVERHEAT\r\n");
    } else {
        if (error_isSet(NS_PROCESSOR_OVERHEAT_POWER) && (processor_temp < (PROCESSOR_INTERNAL_TEMPERATURE_LIMIT - 5.0))) error_clear(NS_PROCESSOR_OVERHEAT_POWER);
    }
}

void temperature_controller_update_internal_temp(void) {
    float internal_temp;
    uint8_t int_temp;
    if (data_getSafeToReadTemp()) {
        data_setSafeToReadTemp(0);

        if (deviceTemp.isReadyToMeasure()) {
            deviceTemp.measureTemperature();
        }

        if (deviceTemp.isReadyToRead()) {
            deviceTemp.readTemperatureToStorage();
        }

        if (deviceTemp.isNewValueAvailable()) {
            internal_temp = deviceTemp.getTemperature();
            if (internal_temp > DEVICE_TEMPERATURE_LIMIT_MAX) {
                if (!error_isSet(NS_DEVICE_OVERHEAT)) error_set(NS_DEVICE_OVERHEAT);
                printf("DEVICE OVERHEAT\r\n");
            } else {
                if (error_isSet(NS_DEVICE_OVERHEAT) && (internal_temp < (DEVICE_TEMPERATURE_LIMIT_MAX - 5.0))) error_clear(NS_DEVICE_OVERHEAT);
            }
            // data.device_temperature = internal_temp;
            int_temp = (uint8_t)internal_temp;
            PowerData_write(TEMPERATURE, &int_temp);
        }
    }
}
