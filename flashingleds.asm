; vim: tabstop=4 shiftwidth=4 expandtab smarttab softtabstop=40
;
; My first program: LEDs are connected to ping 3 & 4 (two pins free
; from programmer in my case), on start only one LED is on and then
; each time period (set in r17 below) both leds are toggled
;
.include "tn13def.inc"

    ldi r16, (1<<PB3)                   ; set pins 3 & 4 as output, pin 3 high
    ldi r17, (1<<PB3)|(1<<PB4)
    out PORTB, r16
    out DDRB, r17

    mov r16, r17                        ; save mask for toggling LEDs

mainloop:
    clr r1                              ; set up loop timing
    clr r2
    ldi r17, 5

wait:
    dec r1
    brne wait
    dec r2
    brne wait
    dec r17
    brne wait

    out PINB, r16                       ; toggle both LEDs

    rjmp mainloop                       ; continue main loop
