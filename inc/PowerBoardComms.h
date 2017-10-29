#ifndef POWER_BOARD_COMMS_H
#define POWER_BOARD_COMMS_H

#include "NectarStream.h"

namespace power_board {  
  void setup();
  void loop();
  void start();
  void stop();
  void enter_test_mode();
}

class powerStream : public IStreamDelegate {
public:
  powerStream(PinName transmitPin, PinName receivePin, bool inverse_logic = false,
                unsigned int buffSize = 64)
      : m_serial(transmitPin, receivePin),
        stream(*this) {}

  ~powerStream(){};

  void setup();
  void Rx_interrupt();

  virtual void write(uint8_t byte);
  virtual void received_power_board_state(const nectar_contract::PowerBoardState &state);
  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
