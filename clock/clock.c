#define F_CPU 1000000 // Hz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "ssd.h"
#include "twi.h"

void error() {
	ssd_hex(0xe);
	while(1) {
	}
}

int main(void) {
	sleep_cpu();
	ssd_init();

	twi_init();

	twi_start_condition();
	if (twi_send_byte(0b11010000) != 0)
		error();
	if (twi_send_byte(0x00) != 0)
		error();
	if (twi_send_byte(0b00000000) != 0)
		error();
	twi_stop_condition();

	while(1) {
		twi_start_condition();
		if (twi_send_byte(0b11010000) != 0)
			error();
		if (twi_send_byte(0x00) != 0)
			error();
		twi_stop_condition();

		twi_start_condition();
		if (twi_send_byte(0b11010001) != 0)
			error();
		uint8_t sec = twi_recv_byte(1);
		twi_stop_condition();

		ssd_dec(sec & 0x0f);
	}

	return 0;
}
