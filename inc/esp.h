#ifndef ESP_H
#define ESP_H

#include "NectarStream.h"

namespace esp {
  extern nectar_contract::MainBoardStats stats;
  extern nectar_contract::NectarStatus status;
  
  void setup();
  void loop();
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
  virtual void received_main_board_stats(const nectar_contract::MainBoardStats &stats);
  virtual void received_esp_status(const nectar_contract::NectarStatus &status);
  NectarStream stream;

private:
  RawSerial m_serial;
};

#endif

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
