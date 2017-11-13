#ifndef POWER_BOARD_COMMS_H
#define POWER_BOARD_COMMS_H

#include "mbed.h"
#include "NectarStream.h"

namespace power_board {  
  void setup();
  void start();
  void stop();
  bool hasReceivedFirstMessage();
}

class powerStream : public IStreamDelegate {
public:
  powerStream(PinName transmitPin, PinName receivePin, bool inverse_logic = false,
                unsigned int buffSize = 64)
      : m_serial(transmitPin, receivePin),
        stream(*this) {}

  virtual ~powerStream(){};

  void setup();
  void Rx_interrupt();

  virtual void write(uint8_t byte);
  virtual void received_power_board_state(const nectar_contract::PowerBoardState &state);
  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif
