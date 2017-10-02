#ifndef POWER_BOARD_H
#define POWER_BOARD_H

#include "NectarStream.h"

namespace power_board {
  extern nectar_contract::PowerBoardStats stats;
  extern nectar_contract::PowerBoardGridMeter grid_meter;
  
  void setup();
  void loop();
  void start();
  void stop();
  void send_grid_meter();
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
    
  virtual void received_power_stats(const nectar_contract::PowerBoardStats &stats);
    
  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
