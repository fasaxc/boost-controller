Boost Controller
================

This repo contains the source code for an AVR-based boost convertor controller.
It uses fast PWM to control the switch and uses the ADC to read the output
voltage (via a voltage divider).  It then adjusts the PWM duty cycle to maintain
a contstant voltage.

I've posted the schematic and step-by step instructions as an instructable:

http://www.instructables.com/id/Make-a-microcontroller-based-boost-converter/

With the right components, it can output several watts at 18V from a 5V supply.
The voltage is adjustable by adjusting the feedback potentiometer.

Code
----

The code is in main.c.  I use the avr-gcc and avr-libc toolchain with AVR 
Eclipse.  Lady Ada has an excellent tutorial on setting up the toolchain 
here: http://www.ladyada.net/learn/avr/.
