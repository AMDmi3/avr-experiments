#ifndef TWI_H
#define TWI_H

#include <util/delay.h>

#include "pin.h"

#define TWI_DELAY() _delay_us(10)

#define TWI_SETCLOCK(val) SETPIN(A, 0, val)
#define TWI_SETDATA(val) SETPIN(A, 1, val)
#define TWI_GETDATA() GETPIN(A, 1)

#define TWI_WRITEMODE() SETOUT(A, 1, 1)
#define TWI_READMODE() do { PULLUP(A, 1, 0); SETOUT(A, 1, 0); } while(0)

void twi_init() {
	// set pins used by 2wire interface
	DDRA |= 0b00000011;
	PORTA |= 0b00000011;
}

void twi_start_condition() {
	// assumes clock high and data high
	TWI_DELAY();
	TWI_SETDATA(0);
	TWI_DELAY();
	TWI_DELAY();
	TWI_SETCLOCK(0);
	TWI_DELAY();
}

void twi_stop_condition() {
	// assumes clock low
	TWI_DELAY();
	TWI_SETDATA(0);
	TWI_DELAY();
	TWI_DELAY();
	TWI_SETCLOCK(1);
	TWI_DELAY();
	TWI_DELAY();
	TWI_SETDATA(1);
	TWI_DELAY();
}

void twi_send_bit(int bit) {
	// assumes clock low
	TWI_SETDATA(bit);
	TWI_DELAY();
	TWI_SETCLOCK(1);
	TWI_DELAY();
	TWI_DELAY();
	TWI_SETCLOCK(0);
	TWI_DELAY();
}

int twi_recv_bit() {
	// assumes clock low
	TWI_DELAY();
	TWI_SETCLOCK(1);
	TWI_DELAY();
	int res = TWI_GETDATA();
	TWI_DELAY();
	TWI_SETCLOCK(0);
	TWI_DELAY();
	return res;
}

int twi_send_byte(uint8_t byte) {
	twi_send_bit(byte & 0b10000000);
	twi_send_bit(byte & 0b01000000);
	twi_send_bit(byte & 0b00100000);
	twi_send_bit(byte & 0b00010000);
	twi_send_bit(byte & 0b00001000);
	twi_send_bit(byte & 0b00000100);
	twi_send_bit(byte & 0b00000010);
	twi_send_bit(byte & 0b00000001);

	TWI_READMODE();
	int res = twi_recv_bit();
	TWI_WRITEMODE();

	return res;
}

uint8_t twi_recv_byte(int last) {
	TWI_READMODE();

	uint8_t byte = 0, i = 0;
	for (i = 0; i < 8; i++) {
		byte <<= 1;
		byte |= twi_recv_bit();
	}

	TWI_WRITEMODE();
	twi_send_bit(last);

	return byte;
}

#endif
