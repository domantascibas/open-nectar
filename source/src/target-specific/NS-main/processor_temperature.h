#ifndef _NS_PROCESSOR_TEMPERATURE_H
#define _NS_PROCESSOR_TEMPERATURE_H

/* Temperature sensor calibration value address */
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7C2))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7B8))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))

void processor_temperature_init(void);
float processor_temperature_measure(void);

#endif
