/*
 /SPI control protocol for
 /Newhaven display character OLED DZW series OLED displays
 /SPI implementation based on code by Alee Shah - Newhaven Display International, Inc.
 /and command set derived from LiquidCrystal by David Mellis
 / Richard Blair
 / 07/2024
 /v 1.0.0
 /delay needed for ESP32 implementation versus Arduino Mega
 /command and write have delayMicroseconds(500) needed to allow Esp32 to slow its communication down
 /send and write4bits have assembler NOP added to cause a 1 clock cycle delay again needed for ESP32 to work reliably
 /should not slow down Arduino boards too much.
 /Arduino mega clock speed: 16 MHz, SPI speed 4 MHz to 8 MHz
 /ESP32 clock speed: 80 MHz, maximum GPIO speed 20 MHz
 /NHD-0420DZW-AY5 minimum scl pulse widh 100 ns or 10 MHz
 
 /
 *
 */

#include "Newhaven_CharacterOLED_SPI.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

Newhaven_CharacterOLED_SPI::Newhaven_CharacterOLED_SPI(uint8_t CS, uint8_t SCK, uint8_t MOSI,
                                     uint8_t MISO)
{
  _CS_pin = CS;
  _SCK_pin = SCK;
  _MOSI_pin = MOSI;
  _MISO_pin = MISO;
}

void Newhaven_CharacterOLED_SPI::init()

{
  pinMode(_CS_pin, OUTPUT);
  pinMode(_SCK_pin, OUTPUT);
  pinMode(_MOSI_pin, OUTPUT);
  pinMode(_MISO_pin, OUTPUT);
  
  digitalWrite(_CS_pin, HIGH);  //deselect display for now
  _displayfunction = OLED_FUNCTIONSET | OLED_4BITMODE;
}

void Newhaven_CharacterOLED_SPI::begin(uint8_t cols, uint8_t lines)
{
  _numlines = lines;
  _currline = 0;
  //initialization sequence form Alee Shah's SPI implementation
    
  digitalWrite(_CS_pin, LOW); //select display
  delay(300); // give it some time to power up
  
  //command(0b00101000);       //Function set
  command(_displayfunction);       //Function set
  delay(2);
  command(0b00001000);       //Display OFF
  delay(2);
  command(0b00001000);       //Display OFF
  delay(2);
  command(0b00000000);       //Display OFF
  delay(2);
  command(0b00000001);       //Clear Display
  delay(2);
  command(0b00000000);       //Display OFF
  delay(2);
  command(0b00000110);       //Entry Mode set
  delay(2);
  command(0b00000000);       //Display OFF
  delay(2);
  command(0b00000010);       //Return Home
  delay(2);
  command(0b00000000);       //Display OFF
  delay(2);
  command(0b00001100);       //Display ON
  delay(2);
  digitalWrite(_MOSI_pin, LOW);
  digitalWrite(_SCK_pin, LOW);
  //digitalWrite(_CS_pin, LOW);
  delay(100);
}


/********** high level commands, for the user! */
void Newhaven_CharacterOLED_SPI::clear()
{
  command(OLED_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  //
}

void Newhaven_CharacterOLED_SPI::home()
{
  command(OLED_RETURNHOME);  // set cursor position to zero
  //  delayMicroseconds(2000);  // this command takes a long time!
}

void Newhaven_CharacterOLED_SPI::setCursor(uint8_t col, uint8_t row)
{
  uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row >= _numlines ) 
  {
    row = 0;  //write to first line if out off bounds
  }
  
  command(OLED_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void Newhaven_CharacterOLED_SPI::noDisplay()
{
  _displaycontrol &= ~OLED_DISPLAYON;
  command(OLED_DISPLAYCONTROL | _displaycontrol);
}
void Newhaven_CharacterOLED_SPI::display()
{
  _displaycontrol |= OLED_DISPLAYON;
  command(OLED_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void Newhaven_CharacterOLED_SPI::noCursor()
{
  _displaycontrol &= ~OLED_CURSORON;
  command(OLED_DISPLAYCONTROL | _displaycontrol);
  _displaycontrol |= OLED_DISPLAYON;  //need to turn the display back on
  command(OLED_DISPLAYCONTROL | _displaycontrol);
  
}
void Newhaven_CharacterOLED_SPI::cursor()
{
  _displaycontrol |= OLED_CURSORON;
  command(OLED_DISPLAYCONTROL | _displaycontrol);
  _displaycontrol |= OLED_DISPLAYON;  //need to turn the display back on
  command(OLED_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void Newhaven_CharacterOLED_SPI::noBlink()
{
  _displaycontrol &= ~OLED_BLINKON;
  command(OLED_DISPLAYCONTROL | _displaycontrol);
  _displaycontrol |= OLED_DISPLAYON;  //need to turn the display back on
  command(OLED_DISPLAYCONTROL | _displaycontrol);
}
void Newhaven_CharacterOLED_SPI::blink()
{
  _displaycontrol |= OLED_BLINKON;
  command(OLED_DISPLAYCONTROL | _displaycontrol);
  _displaycontrol |= OLED_DISPLAYON;  //need to turn the display back on
  command(OLED_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void Newhaven_CharacterOLED_SPI::scrollDisplayLeft(void)
{
  command(OLED_CURSORSHIFT | OLED_DISPLAYMOVE | OLED_MOVELEFT);
}
void Newhaven_CharacterOLED_SPI::scrollDisplayRight(void)
{
  command(OLED_CURSORSHIFT | OLED_DISPLAYMOVE | OLED_MOVERIGHT);
}

// This is for text that flows Left to Right
void Newhaven_CharacterOLED_SPI::leftToRight(void)
{
  _displaymode |= OLED_ENTRYLEFT;
  command(OLED_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void Newhaven_CharacterOLED_SPI::rightToLeft(void)
{
  _displaymode &= ~OLED_ENTRYLEFT;
  command(OLED_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void Newhaven_CharacterOLED_SPI::autoscroll(void)
{
  _displaymode |= OLED_ENTRYSHIFTINCREMENT;
  command(OLED_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void Newhaven_CharacterOLED_SPI::noAutoscroll(void)
{
  _displaymode &= ~OLED_ENTRYSHIFTINCREMENT;
  command(OLED_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Newhaven_CharacterOLED_SPI::createChar(uint8_t location, uint8_t charmap[])
{
  location &= 0x7; // we only have 8 locations 0-7
  command(OLED_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) 
  {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void Newhaven_CharacterOLED_SPI::command(uint8_t value)
{
  send(value, LOW);
  delayMicroseconds(5000); //works without delay on Arduino Mega, delay needed for ESP32
}

inline size_t Newhaven_CharacterOLED_SPI::write(uint8_t value)
{
  send(value, HIGH);
  delayMicroseconds(5000); //works without delay on Arduino Mega, delay needed for ESP32
  return 1; //assume sucess
}

/************ low level data pushing commands **********/

// write either command or data
void Newhaven_CharacterOLED_SPI::send(uint8_t value, uint8_t mode)
{
  
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //  Command Function:
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //  10-bit Instruction Transmission if RS LOW
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  
  uint8_t m;
  //sequence form Alee Shah's SPI implementation
  //start data transmission
  digitalWrite(_CS_pin, LOW);   // CS = 0; select display to write
  //communication preamble
  digitalWrite(_MOSI_pin, mode);  // RS = 0 is 10 bit transmission, RS=1 is 8 bit transmission;
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, HIGH); // Clock RS Pin in
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_MOSI_pin, LOW);  // RW = 0;
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, HIGH); // Clock RW Pin in
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  //send data
  for(m=0;m<8;m++){        // Clock in DB0 - DB7
    if((value&0x80)==0x80) {
      digitalWrite(_MOSI_pin, HIGH);
    }
    else {
      digitalWrite(_MOSI_pin, LOW);
    }
    while(0);
    value=(value<<1);
    digitalWrite(_SCK_pin, LOW);
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    while(0);
    digitalWrite(_SCK_pin, HIGH);
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    while(0);
    digitalWrite(_SCK_pin, LOW);
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  }
  //end data transmission
  digitalWrite(_CS_pin, HIGH);
  // CS = 1; deselect display
}



void Newhaven_CharacterOLED_SPI::write4bits(uint8_t value)
{
  uint8_t m;
  //sequence form Alee Shah's SPI implementation
  //start data transmission
  digitalWrite(_CS_pin, LOW);   // CS = 0; select display to write
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  //communication preamble
  digitalWrite(_MOSI_pin, LOW);  // RS = 0;
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, HIGH); // Clock RS Pin in
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  delay(1);
  digitalWrite(_MOSI_pin, LOW);  // RW = 0;
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, HIGH); // Clock RW Pin in
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  digitalWrite(_SCK_pin, LOW);
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  for(m=0;m<4;m++){        // Clock in DB0 - DB7
    if((value&0x80)==0x80)
      digitalWrite(_MOSI_pin, HIGH);
    else
      digitalWrite(_MOSI_pin, LOW);
    while(0);
    value=(value<<1);
    digitalWrite(_SCK_pin, LOW);
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    while(0);
    digitalWrite(_SCK_pin, HIGH);
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    while(0);
    digitalWrite(_SCK_pin, LOW);
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
    __asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");//single clock cycle delay for ESP32
  }
  //end data transmission
  digitalWrite(_CS_pin, HIGH);  // CS = 1; deselect display
}




