; Improved version of flashingleds: more complex LED flashing pattern,
; uses subroutines
;
.nolist
.include "tn13def.inc"
.list

.macro toggle_led
    sbi PINB,@0
.endm

    ldi r17, (1<<PB3)|(1<<PB4)          ; mask for pins 3 & 4
    out DDRB, r17                       ; set pins 3 & 4 as output

mainloop:
    rcall waithalfsec
    toggle_led PB3
    rcall waithalfsec
    toggle_led PB4
    rjmp mainloop                       ; continue main loop

;
; waithalfsec - waits ~half a second
;
waithalfsec:
    rcall wait
    rcall wait
    rcall wait
    ret

;
; wait - waits ~160 milliseconds
;
wait:
    push r1
    push r2
    clr r1                              ; set up loop timing
    clr r2
waitloop:
    dec r1
    brne waitloop                       ; inner wait loop - 256 iterations
    dec r2
    brne waitloop                       ; outer wait loop - 256 iterations
    pop r2
    pop r1
    ret
