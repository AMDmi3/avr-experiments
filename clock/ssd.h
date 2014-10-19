#ifndef SSD_H
#define SSD_H

#include <util/delay.h>

#include "pin.h"

// seven segment display control
//
//  --a--
// |     |
// f     b
// |     |
//  --g--
// |     |
// e     c
// |     |
//  --d--
//
// A -> D3
// B -> D2
// C -> B1
// D -> B0
// E -> D6
// F -> D4
// G -> D5

void ssd_init() {
	// set pins up (which means leds off)
	PORTB |= 0b00000011;
	PORTD |= 0b01111100;

	// set pins as outputs
	DDRB |= 0b00000011;
	DDRD |= 0b01111100;
}

void ssd_display(int mask) {
	// display one symbol
	// mask holds each segment as a bit
	// starting from LSB: -gfedcba
	SETPIN(D, 3, !(mask & 0b00000001));
	SETPIN(D, 2, !(mask & 0b00000010));
	SETPIN(B, 1, !(mask & 0b00000100));
	SETPIN(B, 0, !(mask & 0b00001000));
	SETPIN(D, 6, !(mask & 0b00010000));
	SETPIN(D, 4, !(mask & 0b00100000));
	SETPIN(D, 5, !(mask & 0b01000000));
}

void ssd_dec(int num) {
	// display decimal digit
	switch (num % 10) {
	case 0x0: ssd_display(0b00111111); break;
	case 0x1: ssd_display(0b00000110); break;
	case 0x2: ssd_display(0b01011011); break;
	case 0x3: ssd_display(0b01001111); break;
	case 0x4: ssd_display(0b01100110); break;
	case 0x5: ssd_display(0b01101101); break;
	case 0x6: ssd_display(0b01111101); break;
	case 0x7: ssd_display(0b00000111); break;
	case 0x8: ssd_display(0b01111111); break;
	case 0x9: ssd_display(0b01101111); break;
	}
}

void ssd_hex(int num) {
	// display hexadecimal digit
	switch (num % 16) {
	case 0xA: ssd_display(0b01110111); break;
	case 0xb: ssd_display(0b01111100); break;
	case 0xC: ssd_display(0b00111001); break;
	case 0xd: ssd_display(0b01011110); break;
	case 0xE: ssd_display(0b01111001); break;
	case 0xF: ssd_display(0b01110001); break;
	default: ssd_dec(num); break;
	}
}

void ssd_hello() {
	int a = 1;
	while (a < 0b01000000) {
		ssd_display(a);
		_delay_ms(100);
		a <<= 1;
	}
}

#endif
