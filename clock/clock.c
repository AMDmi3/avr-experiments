#define F_CPU 1000000 // Hz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "ssd.h"
#include "twi.h"
#include "panic.h"

uint8_t zero = 0;
uint8_t seconds = 0;

int main(void) {
	sleep_cpu();
	ssd_init();

	twi_init();
	if (twi_send_bytes(0b1101000, 0x0, &zero, 1) != 1)
		panic();

	while(1) {
		if (twi_recv_bytes(0b1101000, 0x0, &seconds, 1) != 1)
			panic();
		ssd_dec(seconds & 0x0f);
	}

	return 0;
}
