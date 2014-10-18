; LED toggler which processes interrupts (from button in pin 0)
; and also uses sleep instead of a busyloop to conserve power
; (0.22 mA vs. 0.6 mA with default AVR settings)
;
; LED on pin4 is toggled when interrupt is processed and
; LED on pin3 is toggled when AVR wakes up
;
; in ideal conditions, both LEDs will be toggled simultaneously,
; but in reality they'll likely to go out of sync quite fast
; because of switch bounce, which will generate multiple interrupts
; per wakeup
;
.nolist
.include "tn13def.inc"
.list

    ; Interrupt table
    rjmp reset_handler                  ; RESET
    reti                                ; INT0
    rjmp pcint0_handler                 ; PCINT0
    reti                                ; TIM0_OVF
    reti                                ; EE_RDY
    reti                                ; ANA_COMP
    reti                                ; TIM0_COMPA
    reti                                ; TIM0_COMPB
    reti                                ; WDT
    reti                                ; ADC

reset_handler:
    ldi r16, (1<<PB3)|(1<<PB4)          ; mask for pins 3 & 4
    out DDRB, r16                       ; set pins 3 & 4 as output

    ldi r16, (1<<PB0)                   ; mask for pin 0
    out PORTB, r16                      ; enable pullup for pin 0

    ldi r16, (1<<PCINT0)                ; enable pin change interrupt for pin 0
    out PCMSK, r16

    ldi r16, (1<<PCIE)                  ; enable pin change interrupts
    out GIMSK, r16

    ldi r16, (1<<SE)                    ; enable sleep
    out MCUCR, r16

    ldi r16, (1<<PB3)                   ; mask for pin 3
    ldi r17, (1<<PB4)                   ; mask for pin 4

    sei                                 ; enable interrupts

mainloop:
    sleep
    sbi PINB, PB3                       ; toggle pin 3
    rjmp mainloop

pcint0_handler:
    sbi PINB, PB4                       ; toggle pin 4
    reti
