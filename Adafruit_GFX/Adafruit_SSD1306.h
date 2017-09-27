/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/*
 *  Modified by Neal Horman 7/14/2012 for use in mbed
 */

#ifndef _ADAFRUIT_SSD1306_H_
#define _ADAFRUIT_SSD1306_H_

#include "mbed.h"
#include "Adafruit_GFX.h"

#include <vector>
#include <algorithm>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_I2C_ADDRESS   0x3C  // 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.
    SSD1306_128_64  128x64 pixel display
    SSD1306_128_32  128x32 pixel display
    SSD1306_96_16
    -----------------------------------------------------------------------*/
   #define SSD1306_128_64
//   #define SSD1306_128_32
//   #define SSD1306_96_16
/*=========================================================================*/

#if defined SSD1306_128_64 && defined SSD1306_128_32
  #error "Only one SSD1306 display can be specified at once in SSD1306.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32 && !defined SSD1306_96_16
  #error "At least one SSD1306 display must be specified in SSD1306.h"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH                  96
  #define SSD1306_LCDHEIGHT                 16
#endif

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

// A DigitalOut sub-class that provides a constructed default state
class DigitalOut2 : public DigitalOut
{
public:
	DigitalOut2(PinName pin, bool active = false) : DigitalOut(pin) { write(active); };
	DigitalOut2& operator= (int value) { write(value); return *this; };
	DigitalOut2& operator= (DigitalOut2& rhs) { write(rhs.read()); return *this; };
	operator int() { return read(); };
};


/** The pure base class for the SSD1306 display driver.
 *
 * You should derive from this for a new transport interface type,
 * such as the SPI and I2C drivers.
 */
class Adafruit_SSD1306 : public Adafruit_GFX
{
public:
	Adafruit_SSD1306(PinName RST, uint8_t rawHeight, uint8_t rawWidth)
		: Adafruit_GFX(rawWidth,rawHeight)
		, rst(RST,false)
	{
  };

	void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC);
	
	// These must be implemented in the derived transport driver
	virtual void command(uint8_t c) = 0;
	virtual void data(uint8_t c) = 0;
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

	/// Clear the display buffer    
	void clearDisplay(void);
	virtual void invertDisplay(bool i);

	/// Cause the display to be updated with the buffer content.
	void display();
    
protected:
	virtual void sendDisplayBuffer(uint8_t *buffer) = 0;
	DigitalOut2 rst;
};


/** This is the I2C SSD1306 display driver transport class
 *
 */
class Adafruit_SSD1306_I2c : public Adafruit_SSD1306
{
public:
	#define SSD_I2C_ADDRESS     0x78
	/** Create a SSD1306 I2C transport display driver instance with the specified RST pin name, the I2C address, as well as the display dimensions
	 *
	 * Required parameters
	 * @param i2c - A reference to an initialized I2C object
	 * @param RST - The Reset pin name
	 *
	 * Optional parameters
	 * @param i2cAddress - The i2c address of the display
	 * @param rawHeight - The vertical number of pixels for the display, defaults to 32
	 * @param rawWidth - The horizonal number of pixels for the display, defaults to 128
	 */
	Adafruit_SSD1306_I2c(I2C &i2c, PinName RST, uint8_t i2cAddress = SSD_I2C_ADDRESS, uint8_t rawHeight = 64, uint8_t rawWidth = 128)
	    : Adafruit_SSD1306(RST, rawHeight, rawWidth)
	    , mi2c(i2c)
	    , mi2cAddress(i2cAddress)
	    {
		    begin();
        clearDisplay();
		    //splash();
		    //display();
	    };

	virtual void command(uint8_t c)
	{
		char buff[2];
		buff[0] = 0; // Command Mode
		buff[1] = c;
		mi2c.write(mi2cAddress, buff, sizeof(buff));
	}

	virtual void data(uint8_t c)
	{
		char buff[2];
		buff[0] = 0x40; // Data Mode
		buff[1] = c;
		mi2c.write(mi2cAddress, buff, sizeof(buff));
	};

protected:
	virtual void sendDisplayBuffer(uint8_t *buffer) {
		char buff[17];
		buff[0] = 0x40; // Data Mode

		// send display buffer in 16 byte chunks
		for(uint16_t i=0; i<1024; i+=16 ) 
		{	uint8_t x ;

			// TODO - this will segfault if buffer.size() % 16 != 0
			for(x=1; x<sizeof(buff); x++) 
				buff[x] = buffer[i+x-1];
			mi2c.write(mi2cAddress, buff, sizeof(buff));
		}
	};

	I2C &mi2c;
	uint8_t mi2cAddress;
};

#endif
