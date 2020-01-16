#ifndef SERVICE_COMMS_H
#define SERVICE_COMMS_H

void service_setup(void);
uint8_t service_getFakeTemperature(void);
uint8_t service_newValAvail(void);

extern RawSerial pc;

#endif
