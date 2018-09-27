#ifndef _NS_DEVICE_MODES_H
#define _NS_DEVICE_MODES_H

void device_modes_init(void);
void device_modes_loop(void);
void device_modes_calibrate(void);
  void testingStandCalibrate();

void device_modes_set_state_running(void);
void device_modes_set_state_stop(void);

#endif
