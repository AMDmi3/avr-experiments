; My first program: LEDs are connected to pins 3 & 4 (two pins free
; from programmer in my case), one LED is lit at a time, and with a
; period of 0.5 second LEDs are toggled
;
.nolist
.include "tn13def.inc"
.list

    ldi r16, (1<<PB3)                   ; mask for pin 3
    ldi r17, (1<<PB3)|(1<<PB4)          ; mask for pins 3 & 4
    out PORTB, r16                      ; set pin 3 as high
    out DDRB, r17                       ; set pins 3 & 4 as output

mainloop:
    clr r1                              ; set up loop timing
    clr r2
    ldi r25, 3                          ; delay = outer wait loop iterations
    ; in theory,
    ; outer loop time ~= inner loop time * 256 * 256 / (clock rate / clock div)
    ; inner loop time ~= time(dec) + time(brne which jumps) = 3
    ; with default AVR settings, outer loop time ~=
    ;     3 * 256 * 256 / (9600000 / 8) ~= 0.16 sec
    ; thus 6 iterations give us one second delay

wait:
    dec r1
    brne wait                           ; inner wait loop - 256 iterations
    dec r2
    brne wait                           ; middle wait loop - 256 iterations
    dec r25
    brne wait                           ; outer wait loop - [r25] iterations

    out PINB, r17                       ; toggle both LEDs

    rjmp mainloop                       ; continue main loop
