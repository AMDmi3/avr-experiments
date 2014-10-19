#ifndef PANIC_H
#define PANIC_H

#include <avr/sleep.h>

#include "ssd.h"

void panic() {
	ssd_display(0b00001000);
	sleep_enable();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_cpu();
}

#endif
