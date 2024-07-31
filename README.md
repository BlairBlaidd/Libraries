# Newhaven_CharacterOLED_SPI
Overview
Implementation of SPI communication on Newhaven DZW series of OLED character displays.  These are amazingly bright and readable displays, but come with parallel interface by default.  Port expander implementation over I2C produced unacceptably slow refreshes.  These displays are able to accept SPI input with a few jumper changes.

Compatability<br>
This library should be compatible with all Newhaven Display DZW series<br>
NHD-0220DZW-AY5, 5V 2x20 Character Yellow<br>
NHD-0220DZW-AG5, 5V 2x20 Character Green<br>
NHD-0220DZW-AB5, 5V 2x20 Character Blue<br>
NHD-0420DZW-AY5, 5V 4x20 Character Yellow - TESTED<br>
NHD-0420DZW-AG5, 5V 4x20 Character Green<br>
NHD-0420DZW-AB5, 5V 4x20 Character Blue<br>
NHD-0420DZW-AY3, 3.3V 4x20 Character Yellow<br>
NHD-0420DZW-AG3, 3.3V 4x20 Character Green<br>
NHD-0420DZW-AB3, 3.3V 4x20 Character Blue<br>
<br>
Limitations<br>
This library revision only supports one way communication to the display.v
<br>
Implementation<br>
Please consult the documentation for your display<br>
For a NHD-0420DZW-AY5 you will need to reconfigure the jumper setting with a little soldering.<br>

Move jumper resistor on H_PS_L to connect middle pad to low.
Remove 0 ohm jumper resistor H_CS1_L
Remove 0 ohm jumper resistor H_CS2_L
Add 0 ohm jumper resistor to JCS1 (or your favorite shorting method)
Add 0 ohm jumper resistor to JCS2 (or your favorite shorting method)

Parallel 6800-MPU (page 4 data sheet)
default 
H_PS_L	H
J68_J80	J68
H_SHL_L	H
H_CS1_L	L
H_CS2_L	H
JCS1	X		
JCS2	X

Serial MPU (SPI)
H_PS_L	L
J68_J80	X
H_SHL_L	H
H_CS1_L	OPEN
H_CS2_L	OPEN
JCS1	SHORT
JCS2	SHORT

X - not used 

Quality Control
This library was tested on an Arduino Mega and ESP32-PICO-KIT-1.
ESP32-PICO-KIT-1 implementation was tested for 116 hours without issue.

There were some initial timing issues with display corruption after extended use.  This was mitigated by adding delays in the command, write, and send routines. If you experience issues try increasing the delays in these routines.

The NHD-0420DZW-AY5 is a 5V device and works directly with the Mega.  The ESP32 in not officially 5V compatible, wiser to implement level shifters or choose the NHD-0420DZW-AY3 the 3.3 volt version. A BSS138 level shifter board works well in this application.

