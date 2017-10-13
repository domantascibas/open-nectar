#include "button.h"

Button::Button(PinName input, void (*f)()) {
  InterruptIn interrupt(input);
  interrupt.rise(f);
}

void Button::callback() {
  
}
