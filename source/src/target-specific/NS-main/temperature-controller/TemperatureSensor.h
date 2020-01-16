#ifndef _NS_TEMPERATURE_SENSOR_H
#define _NS_TEMPERATURE_SENSOR_H

void temperatureSensor_init(void);
uint8_t temperatureSensor_isFound(void);
uint8_t temperatureSensor_isNewValAvail(void);
uint8_t temperatureSensor_get(void);
uint8_t temperatureSensor_start(void);

#endif
