#define F_CPU 1000000 // Hz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "ssd.h"
#include "twi.h"
#include "panic.h"

int main(void) {
	sleep_cpu();
	ssd_init();

	uint8_t val = 0;

	twi_init();
	if (twi_send_bytes(0b1101000, 0x0, &val, 1) != 1)
		panic();

	while(1) {
		if (twi_recv_bytes(0b1101000, 0x0, &val, 1) != 1)
			panic();
		ssd_dec(val & 0x0f);
	}

	return 0;
}
