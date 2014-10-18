# AVR experiments

This repo contains code for my experiments with AVR microcontrollers

It start with simplest "flashing LEDs" code and will grow with
programs using more features of AVR (inputs, interrupts, ADC, TWM
etc.).

## Programs

* **flashingleds** - flashes two LEDs
* **subroutines** - the same, uses subroutines
* **interrupts** - demonstrates sleep and interrupt handling
* **multitasking** - demonstrates basic multitasking (threads)

## Building

I use AVRA to build firmware from assembly code and AVRDUDE to flash
the AVR. Makefile is provided to automate these tasks, tuned for my
FreeBSD system.

## Links

* [ATtiny13 datasheet](http://www.atmel.com/Images/doc2535.pdf)
* [ATtiny2313 datasheet](http://www.atmel.com/Images/doc2543.pdf)
* [AVRA assembler](http://avra.sourceforge.net/)
* [AVRDUDE flasher](http://www.nongnu.org/avrdude/)
