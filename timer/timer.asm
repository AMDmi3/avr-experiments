; LED toggler which uses timer (mostly similar to interrupts.asm)
;
; here interrupt period is
;
; timer resolution * timer prescaler / (clock rate / clock div) =
; 256 * 1024 / (9600000 / 8) ~= 0.22 sec
;
.nolist
.include "tn13def.inc"
.list

    ; Interrupt table
    rjmp reset_handler                  ; RESET
    reti                                ; INT0
    reti                                ; PCINT0
    rjmp tim0_ovf_handler               ; TIM0_OVF
    reti                                ; EE_RDY
    reti                                ; ANA_COMP
    reti                                ; TIM0_COMPA
    reti                                ; TIM0_COMPB
    reti                                ; WDT
    reti                                ; ADC

reset_handler:
    ldi r16, (1<<PB3)|(1<<PB4)          ; mask for pins 3 & 4
    out DDRB, r16                       ; set pins 3 & 4 as output

    ldi r16, 0b0000101                  ; set 1/1024 prescaler for I/O clock
    out TCCR0B, r16

    ldi r16, (1<<TOIE0)                 ; enable timer interrupts
    out TIMSK0, r16

    ldi r16, (1<<SE)                    ; enable sleep
    out MCUCR, r16

    ldi r16, (1<<PB3)                   ; mask for pin 3
    ldi r17, (1<<PB4)                   ; mask for pin 4

    sei                                 ; enable interrupts

mainloop:
    sleep
    sbi PINB, PB3                       ; toggle pin 3
    rjmp mainloop

tim0_ovf_handler:
    sbi PINB, PB4                       ; toggle pin 4
    reti
