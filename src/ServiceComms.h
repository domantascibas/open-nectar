#ifndef SERVICE_COMMS_H
#define SERVICE_COMMS_H

namespace service {
  extern float fakeTemperature;
  extern bool newValueAvailable;
  
  void setup();
  float getFakeTemperature();
  bool isNewValueAvailable();
}

extern RawSerial pc;

#endif
