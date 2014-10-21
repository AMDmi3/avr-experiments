#ifndef I2C_H
#define I2C_H

#include <util/delay.h>

#include "pin.h"

#define I2C_DELAY() _delay_us(10)

#define I2C_SETCLOCK(val) SETPIN(A, 0, val)
#define I2C_SETDATA(val) SETPIN(A, 1, val)
#define I2C_GETDATA() GETPIN(A, 1)

#define I2C_WRITEMODE() SETOUT(A, 1, 1)
#define I2C_READMODE() do { PULLUP(A, 1, 0); SETOUT(A, 1, 0); } while(0)

static void i2c_init() {
	// set pins used by 2wire interface
	DDRA |= 0b00000011;
	PORTA |= 0b00000011;
}

static void i2c_start_condition() {
	// assumes clock high and data high
	I2C_SETDATA(0);
	I2C_DELAY();
	I2C_SETCLOCK(0);
	I2C_DELAY();
}

static void i2c_stop_condition() {
	// assumes clock low
	I2C_SETDATA(0);
	I2C_DELAY();
	I2C_SETCLOCK(1);
	I2C_DELAY();
	I2C_SETDATA(1);
	I2C_DELAY();
}

static void i2c_send_bit(int bit) {
	// assumes clock low
	I2C_SETDATA(bit);
	I2C_DELAY();
	I2C_SETCLOCK(1);
	I2C_DELAY();
	I2C_SETCLOCK(0);
	I2C_DELAY();
}

static int i2c_recv_bit() {
	// assumes clock low
	I2C_SETCLOCK(1);
	I2C_DELAY();
	int res = I2C_GETDATA();
	I2C_SETCLOCK(0);
	I2C_DELAY();
	return res;
}

static int i2c_send_byte(uint8_t byte) {
	for (int i = 0; i < 8; i++) {
		i2c_send_bit(byte & 0b10000000);
		byte <<= 1;
	}

	I2C_READMODE();
	int res = i2c_recv_bit();
	I2C_WRITEMODE();

	return res;
}

static uint8_t i2c_recv_byte(int last) {
	I2C_READMODE();

	uint8_t byte = 0, i = 0;
	for (i = 0; i < 8; i++) {
		byte <<= 1;
		byte |= i2c_recv_bit();
	}

	I2C_WRITEMODE();
	i2c_send_bit(last);

	return byte;
}

static int i2c_send_bytes(uint8_t address, uint8_t offset, uint8_t* bytes, int count) {
	int ret = -1;
	i2c_start_condition();
	if (i2c_send_byte(address << 1) != 0)
		goto ret;
	if (i2c_send_byte(offset) != 0)
		goto ret;
	for (ret = 0; ret < count; ++ret) {
		if (i2c_send_byte(bytes[ret]) != 0)
			goto ret;
	}
ret:
	i2c_stop_condition();
	return ret;
}

static int i2c_recv_bytes(uint8_t address, uint8_t offset, uint8_t* bytes, int count) {
	int ret = -1;
	uint8_t* current_byte = bytes;

	if (i2c_send_bytes(address, offset, 0, 0) == -1)
		goto ret;

	i2c_start_condition();
	if (i2c_send_byte((address << 1) | 1) != 0)
		goto ret;
	for (ret = 0; ret < count; ++ret)
		*current_byte = i2c_recv_byte(ret == count - 1);
ret:
	i2c_stop_condition();
	return ret;
}

#endif
