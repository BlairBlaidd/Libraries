/*
 /SPI control protocol for
 /Newhaven display character OLED DZW series OLED displays
 /SPI implementation based on code by Alee Shah - Newhaven Display International, Inc.
 /and command set derived from LiquidCrystal by David Mellis
 / Richard Blair
 / 07/2024
 /tested with a NHD-0420DZW-AY5
 /Arduino Mega 
 /ESP32-PICO-DEVKITM-2 with BSS138 FET level shifter to shift signals going to display to 5V.
 /The libary has delays added to allow ESP32 operation.
 /ident, mega, wire ,display ,esp32wire , esp32
 /MOSI(SPID)  - display pin 14
 /MISO(SPIQ) unused
 /SCK - display pin 12
 /CS display pin, 15
 /+5V, display pin 2
 /GND,  display pin 1
 */

/****************************************************
*      PINOUT: Controller -> Character OLED        *
****************************************************/
#include <Newhaven_CharacterOLED_SPI.h>
 
//Arduino mega pins
//#define CS    12
//#define MOSI   11
//#define MISO 10
//#define SCK   9

//ESP32 pico
#define CS    15
#define MOSI   13
#define MISO 27
#define SCK   14

// initialize the library with the OLED hardware 
Newhaven_CharacterOLED_SPI oled(CS, SCK, MOSI, MISO); 
long StartTime;
long theTime=0;

void setup() 
{
  // Print a message to the LCD.
  oled.init();
  oled.begin(20, 4);
  oled.clear();
  oled.home();
  oled.print("hello SPI OLED World");
  StartTime=millis();
  oled.setCursor(0, 1);
  oled.print(theTime);
}

void loop() 
{
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  oled.setCursor(0, 1);
  // print the number of seconds since reset:
  if ((millis()-StartTime)>1000) {
    theTime+=1;
    oled.print(theTime);
    StartTime=millis();
  }
}
