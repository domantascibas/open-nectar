#ifndef _NS_MAIN_BOARD_COMMS_H
#define _NS_MAIN_BOARD_COMMS_H

#include "mbed.h"
#include "NectarStream.h"

void main_board_comms_init(void);

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
