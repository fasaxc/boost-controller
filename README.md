Speaker Controller
==================

This repo contains the source code for an AVR-based boost convertor controller.
It uses fast PWM to control the switch and uses the ADC to read the output
voltage (via a voltage divider).  It then adjusts the PWM duty cycle to maintain
a contstant voltage.

The code's not much use without the schematic, which I'll try to publish 
as soon as I've got it finalised!

Code
----

The code is in main.cpp.  I use the avr-gcc and avr-libc toolchain with AVR 
Eclipse.  Lady Ada has an excellent tutorial on setting up the toolchain 
here: http://www.ladyada.net/learn/avr/.
