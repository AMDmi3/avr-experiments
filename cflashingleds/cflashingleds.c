#define F_CPU 1000000 // Hz

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	int pins = (1 << PB3) | (1 << PB4);
	PORTB = (1 << PB3);
	DDRB = (1 << DDB3) | (1 << DDB4);
	while (1) {
		PINB = (1 << PINB3) | (1 << PINB4);
		_delay_ms(1000);
	}
	return 0;
}
