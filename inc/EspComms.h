#ifndef ESP_COMMS_H
#define ESP_COMMS_H

#include "mbed.h"
#include "NectarStream.h"

namespace esp {
  void setup();
  void get_data_ISR();
}

class mbedStream : public IStreamDelegate {
public:
  mbedStream(PinName transmitPin, PinName receivePin, bool inverse_logic = false,
                unsigned int buffSize = 64)
      : m_serial(transmitPin, receivePin),
        stream(*this) {}

  virtual ~mbedStream(){};

  void setup();
  void Rx_interrupt();

  virtual void write(uint8_t byte);
  virtual void received_esp_state(const nectar_contract::ESPState &state);
  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif
