#ifndef _NS_SENSOR_CONTROLLER_H
#define _NS_SENSOR_CONTROLLER_H

void sensor_controller_init(void);
void sensor_controller_measure(void);
void sensor_controller_calibrate(void);
void sensor_controller_save_meters(void);

float sensor_controller_get_voltage_ref(void);
float sensor_controller_get_current_ref(void);

#endif
