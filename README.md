# AVR experiments

This repo contains code for my experiments with AVR microcontrollers

It start with simplest "flashing LEDs" code and will grow with
programs using more features of AVR (inputs, interrupts, ADC, TWM
etc.).

## Programs

* **flashingleds.asm** - flashes two LEDs
* **subroutines.asm** - the same, uses subroutines
* **interrupts.asm** - demonstrates sleep and interrupt handling
* **multitasking.asm** - demonstrates basic multitasking (threads)

## Building

I use AVRA to build firmware from assembly code and AVRDUDE to flash
the AVR. Makefile is provided to automate these tasks, tuned for my
FreeBSD system.

## Programmer

I use custom LPT programmer described through the link below.

AVRDUDE config for it is included in the repository.

## Links

* [ATtiny13 datasheet](http://www.atmel.com/Images/doc2535.pdf)
* [simple LPT programmer](http://www.getchip.net/posts/delaem-lpt-programmator-dlya-avr-mikrokontrollerov/) (rus)
* [AVRA assembler](http://avra.sourceforge.net/)
* [AVRDUDE flasher](http://www.nongnu.org/avrdude/)
