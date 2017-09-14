#include "mbed.h"
#include "data.h"
#include "Adafruit_SSD1306.h"
#include "OpenSansLight44pt.h"

InterruptIn keyUp(PA_5);        //KEY_L
InterruptIn keyCenter(PA_6);    //KEY_C
InterruptIn keyDown(PA_7);      //KEY_R

class I2CPreInit : public I2C {
  public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl) {
      frequency(400000);
      start();
    };
};
 
I2CPreInit oled_i2c(PB_7, PB_6);
Adafruit_SSD1306_I2c oled(oled_i2c, PC_13);

namespace screen {
  void setup() {
    oled.setTextColor(WHITE);
    oled.clearDisplay();
    oled.printf("Gathering Nectar...");
    oled.display();
  }
  
  void loop() {
    oled.clearDisplay();
    printf("OpenSans 44pt\r\n");
    oled.setFont(&OpenSans_Light44pt7b);
    oled.setCursor(15, -56);
    oled.printf("%2.0f", data.temp_boiler);
    //printf("yAdv %d\r\n", oled.gfxFont->yAdvance);
    //printf("X: %d Y: %d\r\n", oled.getCursorX(), oled.getCursorY());
    oled.display();
  }
}

// *******************************Nectar Sun Copyright © Nectar Sun 2017*************************************   
