; vim: tabstop=4 shiftwidth=4 expandtab smarttab softtabstop=40
;
; My first program: LEDs are connected to ping 3 & 4 (two pins free
; from programmer in my case), on start only one LED is on and then
; each time period (set in r17 below) both leds are toggled
;
.include "tn13def.inc"

    ldi r16, (1<<PB3)                   ; mask for pin 3
    ldi r17, (1<<PB3)|(1<<PB4)          ; mask for pins 3 & 4
    out PORTB, r16                      ; set pin 3 as high
    out DDRB, r17                       ; set pins 3 & 4 as output

mainloop:
    clr r1                              ; set up loop timing
    clr r2
    ldi r25, 5                          ; delay = outer wait loop iterations
    ; outer loop time ~= inner_loop_time * 256 * 256 / (clock_rate / clock_div)
    ; inner_loop_time ~= time(dec) + time(brne which jumps) = 3
    ; with default AVR settings, outer_loop_time ~=
    ;     3 * 256 * 256 / (8000000 / 8) ~= 0.2 sec

wait:
    dec r1
    brne wait                           ; inner wait loop - 256 iterations
    dec r2
    brne wait                           ; middle wait loop - 256 iterations
    dec r25
    brne wait                           ; outer wait loop - [r25] iterations

    out PINB, r17                       ; toggle both LEDs

    rjmp mainloop                       ; continue main loop
