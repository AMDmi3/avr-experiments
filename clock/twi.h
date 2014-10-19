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
	TWI_SETDATA(0);
	TWI_DELAY();
	TWI_SETCLOCK(0);
	TWI_DELAY();
}

void twi_stop_condition() {
	// assumes clock low
	TWI_SETDATA(0);
	TWI_DELAY();
	TWI_SETCLOCK(1);
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
	TWI_SETCLOCK(0);
	TWI_DELAY();
}

int twi_recv_bit() {
	// assumes clock low
	TWI_SETCLOCK(1);
	TWI_DELAY();
	int res = TWI_GETDATA();
	TWI_SETCLOCK(0);
	TWI_DELAY();
	return res;
}

int twi_send_byte(uint8_t byte) {
	for (int i = 0; i < 8; i++) {
		twi_send_bit(byte & 0b10000000);
		byte <<= 1;
	}

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

int twi_send_bytes(uint8_t address, uint8_t offset, uint8_t* bytes, int count) {
	int ret = -1;
	twi_start_condition();
	if (twi_send_byte(address << 1) != 0)
		goto ret;
	if (twi_send_byte(offset) != 0)
		goto ret;
	for (ret = 0; ret < count; ++ret) {
		if (twi_send_byte(bytes[ret]) != 0)
			goto ret;
	}
ret:
	twi_stop_condition();
	return ret;
}

int twi_recv_bytes(uint8_t address, uint8_t offset, uint8_t* bytes, int count) {
	int ret = -1;
	uint8_t* current_byte = bytes;

	if (twi_send_bytes(address, offset, 0, 0) == -1)
		return -1;

	twi_start_condition();
	if (twi_send_byte((address << 1) | 1) != 0)
		goto ret;
	for (ret = 0; ret < count; ++ret)
		*current_byte = twi_recv_byte(ret == count - 1);
ret:
	twi_stop_condition();
	return ret;
}

#endif
