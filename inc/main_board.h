#ifndef MAIN_BOARD_H
#define MAIN_BOARD_H

#include "NectarStream.h"

namespace main_board {
  extern nectar_contract::PowerBoardState state;  
  void setup();
}

class mbedStream : public IStreamDelegate {
public:
  mbedStream(PinName transmitPin, PinName receivePin, bool inverse_logic = false,
                unsigned int buffSize = 64)
      : m_serial(transmitPin, receivePin),
        stream(*this) {}

  ~mbedStream(){};

  void setup();
  void Rx_interrupt();

  virtual void write(uint8_t byte);
  virtual StreamObject get_power_board_state();
  virtual void received_main_board_state_for_power(const nectar_contract::MainBoardStateForPower &state);
  
  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif

// *******************************Nectar Sun Copyright � Nectar Sun 2017*************************************   
