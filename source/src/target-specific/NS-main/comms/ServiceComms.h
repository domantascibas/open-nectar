#ifndef SERVICE_COMMS_H
#define SERVICE_COMMS_H

uint8_t service_newValAvail(void);

namespace service {
extern float fakeTemperature;
extern bool newValueAvailable;

void setup();
float getFakeTemperature();
bool isNewValueAvailable();
}

extern RawSerial pc;

#endif
