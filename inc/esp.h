#ifndef ESP_H
#define ESP_H

#include "NectarStream.h"

namespace esp {
  extern nectar_contract::MainBoardStats stats;
  extern nectar_contract::NectarStatus status;
  
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
  virtual void received_cmd_stats();
  virtual void received_status(const nectar_contract::NectarStatus &status);
  virtual void received_time(const nectar_contract::NectarTime &time);

  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
