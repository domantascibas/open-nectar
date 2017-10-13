#ifndef BUTTON_H
#define BUTTON_H

#include "mbed.h"

struct Button {
  Button(PinName input, void (*f)());
  
  private:
    Ticker t;
    void callback();
};

#endif
