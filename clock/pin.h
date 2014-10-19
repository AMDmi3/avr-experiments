#ifndef PIN_H
#define PIN_H

#include <avr/io.h>

// simplified pin I/O
//
// Synopsis:
//   SETREG(PORT, A, 4, 1) -> sets bit 4 of PORTA to 1
//   SETREG(DDR, D, 3, 0) -> sets bit 3 of DDRD to 0

#define SETREG(reg, port, npin, val) do { if (val) reg ## port |= 1 << npin; else reg ## port &= ~(1 << npin); } while(0)
#define GETREG(reg, port, npin) (!!(reg ## port & (1 << npin)))

#define SETOUT(port, npin, val) SETREG(DDR, port, npin, val)

// output
#define SETPIN(port, npin, val) SETREG(PORT, port, npin, val)
#define TOGGLEPIN(port, npin) SETREG(PIN, port, npin, 1)

// input
#define GETPIN(port, npin) GETREG(PIN, port, npin)
#define PULLUP(port, npin, val) SETREG(PORT, port, npin, val)

#endif
