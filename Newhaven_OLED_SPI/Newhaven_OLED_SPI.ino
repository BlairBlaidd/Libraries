/*
 /SPI control protocol for
 /Newhaven display character OLED DZW series OLED displays
 /SPI implementation based on code by Alee Shah - Newhaven Display International, Inc.
 /and command set derived from LiquidCrystal by David Mellis
 / Richard Blair
 / 07/2024
 /This example demontrates the functionality of the display including custom characters
 /tested with a NHD-0420DZW-AY5
 /Arduino Mega 
 /ESP32-PICO-DEVKITM-2 with BSS138 FET level shifter to shift signals going to display to 5V.
 /The libary has delays added to allow ESP32 operation.
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

Newhaven_CharacterOLED_SPI lcd(CS, SCK, MOSI, MISO); 


/****************************************************
*                 Text Strings                      *
****************************************************/

char const text1[] = ("  Newhaven Display  ");
char const text2[] = ("   International    ");
char const text3[] = ("   CHARACTER TEST   ");
char const text4[] = ("  Serial Interface  ");
char const text5[] = ("ABCDEFGHIJKLMOPQRSTU");
char const text6[] = ("VWXYZabcdefghijklmno");
char const text7[] = ("pqrstuvwxyz123456789");
char const text8[] = (" <(' ')> || <(' ')> ");
byte C0[] = {
  B01110,
  B10101,
  B10101,
  B10101,
  B10001,
  B10001,
  B01110,
  B00000
};

byte C1[] = {
  B01110,
  B10011,
  B10011,
  B10101,
  B10001,
  B10001,
  B01110,
  B00000
};

byte C2[] = {
  B01110,
  B10001,
  B10001,
  B10111,
  B10001,
  B10001,
  B01110,
  B00000
};

byte C3[] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B10011,
  B10011,
  B01110,
  B00000
};

byte C4[] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B10101,
  B10101,
  B01110,
  B00000
};

byte C5[] = {
  B01110,
  B10001,
  B10001,
  B10101,
  B11001,
  B11001,
  B01110,
  B00000
};

byte C6[] = {
  B01110,
  B10001,
  B10001,
  B11101,
  B10001,
  B10001,
  B01110,
  B00000
};

byte C7[] = {
  B01110,
  B11001,
  B11001,
  B10101,
  B10001,
  B10001,
  B01110,
  B00000
};

void disp1(){                 // DISPLAY TEXT
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0); // First line
  lcd.print(text1);
/******************************************/    
  lcd.setCursor(0,1);             // Second Line
  lcd.print(text2);
/******************************************/     
  lcd.setCursor(0,2);              // Third Line
  lcd.print(text3);
/******************************************/   
  lcd.setCursor(0,3);              // Fourth Line
  lcd.print(text4);
}


void disp2(){                 // DISPLAY TEXT
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0); // First Line
  lcd.print(text5);
  lcd.setCursor(0,1);              // Second Line
  lcd.print(text6);
  lcd.setCursor(0,2);              // Second Line
  lcd.print(text7);
  lcd.setCursor(0,3);              // Second Line
  lcd.print(text8);
  //end_data_transmission();  
}

/*****************************************************
*           Setup Function, to run once              *
*****************************************************/

void setup() {
  lcd.init();
  lcd.begin(20, 4);
  lcd.clear();
  lcd.createChar(0, C0);
  lcd.home();
  lcd.createChar(1, C1);
  lcd.home();
  lcd.createChar(2, C2);
  lcd.home();
  lcd.createChar(3, C3);
  lcd.home();
  lcd.createChar(4, C4);
  lcd.home();
  lcd.createChar(5, C5);
  lcd.home();
  lcd.createChar(6, C6);
  lcd.home();
  lcd.createChar(7, C7);
  lcd.clear();
  lcd.home();
}

/*****************************************************
*           Loop Function, to run repeatedly         *
*****************************************************/

void loop() {
  String thetext="";
  float thenumber;
  byte i,j;
  
  disp1();
  delay(2500);
  disp2();
  delay(2500);
  
  lcd.clear();
  
  lcd.setCursor(0,0);
  thetext="Line 0";
  lcd.print(thetext);
  delay(2500);
  
  lcd.setCursor(10,1);
  lcd.rightToLeft(); //reverse text direction
  thetext="Line 1";
  lcd.print(thetext);
  delay(2500);
  lcd.leftToRight(); //set the direction back to normal
  
  lcd.setCursor(2,2);
  thetext="Line 2";
  lcd.print(thetext);
  lcd.setCursor(2,2);
  lcd.blink(); //blink the cursor position
  delay(2500);
  lcd.noBlink(); //turn off blink

  lcd.setCursor(3,3);
  thetext="Line 3";
  thenumber=32.87;
  lcd.print(thenumber);
  lcd.setCursor(4,3);
  lcd.cursor(); //shown underscore cursor below character
  delay(2500);
  lcd.noCursor();//turn underscore off

  i=0;
  j=0;
  
  //display custom character spinning watch
  while (i<8) {
    while (j<8) {
      lcd.setCursor(10,0);
      lcd.write(j);
      delay(250);
      j+=1;
    }
    i+=1;
    j=0;
  }
}
