#define F_CPU 1000000 // Hz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "ssd.h"
#include "twi.h"

int main(void) {
	sleep_cpu();
	ssd_init();
	//ssd_hello();

	/*int counter = 0;
	while (1) {
		ssd_hex(counter++);
		if (counter == 16)
			counter = 0;
		_delay_ms(1000);
	}*/

	twi_init();

	twi_start_condition();

	int res = twi_send_byte(0b11010000);

	twi_stop_condition();

	ssd_dec(res);

	while(1) {
	}

	return 0;
}
