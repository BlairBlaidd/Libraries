/*
 /SPI control protocol for
 /Newhaven display character OLED DZW series OLED displays
 /SPI implementation based on code by Alee Shah - Newhaven Display International, Inc.
 /and command set derived from LiquidCrystal by David Mellis
 / Richard Blair
 / 07/2024
 / v 1.0.0
 *
 */

#ifndef Newhaven_CharacterOLED_SPI_h
#define Newhaven_CharacterOLED_SPI_h

#include <inttypes.h>
#include "Print.h"

// commands listed as binary to facilitate datasheet crossref
#define OLED_CLEARDISPLAY   0b00000001
#define OLED_RETURNHOME     0b00000010
#define OLED_ENTRYMODESET   0b00000100
#define OLED_DISPLAYCONTROL 0b00001000
#define OLED_CURSORSHIFT    0b00010000
#define OLED_FUNCTIONSET    0b00101000
#define OLED_SETCGRAMADDR   0b01000000
#define OLED_SETDDRAMADDR   0b10000000

// flags for display entry mode
#define OLED_ENTRYRIGHT          0b00000000
#define OLED_ENTRYLEFT           0b00000010
#define OLED_ENTRYSHIFTINCREMENT 0b00000001
#define OLED_ENTRYSHIFTDECREMENT 0b00000000

// flags for display on/off control
#define OLED_DISPLAYON  0b00000100
#define OLED_DISPLAYOFF 0b00000000
#define OLED_CURSORON   0b00000010
#define OLED_CURSOROFF  0b00000000
#define OLED_BLINKON    0b00000001
#define OLED_BLINKOFF   0b00000000
//blink and cursor interdependant
//00001DCB
//D=display on/off, C= line cursor on/off, B=blink character on/off

// flags for display/cursor shift
#define OLED_DISPLAYMOVE 0b00001000
#define OLED_CURSORMOVE  0b00000000
#define OLED_MOVERIGHT   0b00000100
#define OLED_MOVELEFT    0b00000000

// flags for function set
#define OLED_8BITMODE    0b00010000
#define OLED_4BITMODE    0b00000000
#define OLED_JAPANESE    0b00000000
#define OLED_EUROPEAN_I  0b00000001
#define OLED_RUSSIAN     0b00000010
#define OLED_EUROPEAN_II 0b00000011


class Newhaven_CharacterOLED_SPI : public Print {
public:
  Newhaven_CharacterOLED_SPI(uint8_t CS, uint8_t SCK, uint8_t MOSI,
		uint8_t MISO);
  void init();
  void begin(uint8_t cols, uint8_t rows);
  
  void clear();
  void home();
  void noDisplay();
  void display();
  void setCursor(uint8_t, uint8_t);
  
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();
  
  void createChar(uint8_t, uint8_t[]);
  
  virtual size_t write(uint8_t);
  void command(uint8_t);
  void write4bits(uint8_t);
  
private:
  void send(uint8_t, uint8_t);
  uint8_t _CS_pin; // LOW: trigger device into listen mode
  uint8_t _SCK_pin;
  uint8_t _MOSI_pin; // serial data line
  uint8_t _MISO_pin;   // unused in talk only mode

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _currline;
  uint8_t _numlines;
};

#endif
