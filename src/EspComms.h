#ifndef _NS_ESP_COMMS_H
#define _NS_ESP_COMMS_H

#include "consts.h"
#include "NectarStream.h"

namespace esp {
  void setup();
  void send_message();
  bool is_message_valid(const nectar_contract::ESPState &state);
  bool receivedFirstMessage();
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
