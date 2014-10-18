; Multitasking example
;
; We run 4 threads of execution in parallel. Of course, since
; AVR is single core only one thread is running at a time, but
; they are swithed so frequently that it would seem that they're
; exeuting simultaneouslt.
;
; The memory is divided into per-thread chunks. Thread stack
; grows down from the highest address of a chunk, and lower
; addresses are used to store thread context
;
; Timer interrupt is used to switch between threads. There we
; save stack pointer, status register and all registers used
; by any thread, switch thread and restore the same stuff for
; it. In this example, each thread needs:
; stack: 2 return addressses (call + interrupt) + 2 pushes = 
;            = 6 bytes
; context: SPL + SREG + r0 + r1 + r16 = 5 bytes
; total: 11 bytes, e.g. we can even have 5 threads with 1 more
; byte for each
; We can have more threads if less stak is used and/or less
; registers shared (also each thread can use different amount
; of memory based on stack size; context size is constant though)
;
; To prepare thread for exection, we only need to store two
; things:
; - its start address into its stack
; - its stack pointer into beginning of its memory chunk
; other elements will be read from memory on first switch
; and their contents are undefined (or zeroes?). The SRAM
; may be zeroed out initially if required
;
; Note that threads are forbidden to use Z and r17 registers
; as these are used for scheduling
;
; Possible room for improvement:
; - yield function which runs schedule immediately to switch
;   to the next thread (useful when current thread has nothing
;   to do)
; - dynamic thread allocation/destruction (may store per-thread
;   flag indicating that the thread is active; destruction resets
;   this flag, thread allocation sets it and setups thread data
;   (flag should be 3-state: disabled/initializing/ready so context
;   switch within new thread initialization is handled properly)
; - fork call may be possible which just duplicates current thread
;   memory into another slot (fixing SPL there and also setting some
;   flag to distinguish forked thread from its parent)
; - sleep support: sheduler should determine a condition when there
;   are no thread to run or when all threads yielded before another
;   timer interrupt arrived. In this case it should just call sleep
;
.nolist
.include "tn13def.inc"
.list

.equ THREAD_MEM = 0x10                  ; amount of per-thread memory

; start addresses of each thread memory
.equ THREAD3_START = low(RAMEND) - THREAD_MEM * 4 + 1
.equ THREAD2_START = low(RAMEND) - THREAD_MEM * 3 + 1
.equ THREAD1_START = low(RAMEND) - THREAD_MEM * 2 + 1
.equ THREAD0_START = low(RAMEND) - THREAD_MEM * 1 + 1

; first and last thread pointers
; note: FIRST is the thread with lowest memory address
.equ FIRST_THREAD_START = THREAD3_START
.equ LAST_THREAD_START = THREAD0_START

    ; Interrupt table
    rjmp main_thread                    ; RESET
    reti                                ; INT0
    reti                                ; PCINT0
    rjmp schedule                       ; TIM0_OVF
    reti                                ; EE_RDY
    reti                                ; ANA_COMP
    reti                                ; TIM0_COMPA
    reti                                ; TIM0_COMPB
    reti                                ; WDT
    reti                                ; ADC

;
; timer interrupt: handle scheduling
;
schedule:
    ; store current thread context
    in r17, SPL                         ; store stack pointer
    st Z, r17
    in r17, SREG                        ; store sreg
    std Z+1, r17
    std Z+2, r1                         ; store other registers used in threads
    std Z+3, r2
    std Z+4, r16

    ; switch to next thread
    cpi ZL, LAST_THREAD_START
    breq to_first_thread
    ldi r17, THREAD_MEM                 ; not on last thread->switch to next thread
    add ZL, r17
    rjmp switched_thread
to_first_thread:
    ldi ZL, FIRST_THREAD_START          ; on last thread->switch to first thread
switched_thread:

    ; load next thread context
    ld r17, Z                           ; restore stack pointer
    out SPL, r17
    ldd r17, Z+1                        ; restore SREG
    cbr r17, (1<<SREG_I)                ; always clear interrupt flag
    out SREG, r17
    ldd r1, Z+2                         ; restore other registers used in threads
    ldd r2, Z+3
    ldd r16, Z+4

    reti                                ; continue execution of thread swithed to

main_thread:
    clr ZH                              ; won't need due to small memory

    ;
    ; setup threads (stack pointer and return address should be
    ; pre-set for each thread other than current one)
    ;

    ; thread1
    ldi ZL, THREAD1_START
    ldi r16, THREAD1_START + THREAD_MEM - 1 ; setup stack
    out SPL, r16
    ldi r16, low(thread1)
    push r16
    ldi r16, high(thread1)
    push r16
    in r16, SPL                         ; store stack pointer
    st Z, r16

    ; thread2
    ldi ZL, THREAD2_START
    ldi r16, THREAD2_START + THREAD_MEM - 1 ; setup stack
    out SPL, r16
    ldi r16, low(thread2)
    push r16
    ldi r16, high(thread2)
    push r16
    in r16, SPL                         ; store stack pointer
    st Z, r16

    ; thread3
    ldi ZL, THREAD3_START
    ldi r16, THREAD3_START + THREAD_MEM - 1 ; setup stack
    out SPL, r16
    ldi r16, low(thread3)
    push r16
    ldi r16, high(thread3)
    push r16
    in r16, SPL                         ; store stack pointer
    st Z, r16

    ; thread0 (this thread; we only need to restore ZL and SPL)
    ldi ZL, THREAD0_START               ; current thread memory pointer
    ldi r16, THREAD0_START + THREAD_MEM - 1
    out SPL, r16                        ; current thread stack

    ; other initialization work
    ldi r16, 0b0000010                  ; set 1/8 prescaler for I/O clock
    out TCCR0B, r16

    ldi r16, (1<<TOIE0)                 ; enable timer interrupts
    out TIMSK0, r16

    sei                                 ; enable interrupts (essentially start scheduling)

    ; fall-through to thread0 code

    ;
    ; Thread 0: blinks with LED on pin 3
    ;
thread0:
    sbi DDRB, PB3
thread0_loop:
    sbi PINB, PB3
    rcall wait
    mov r1, r3
    rjmp thread0_loop

    ;
    ; Thread 1: blinks with LED on pin 4
    ;
thread1:
    sbi DDRB, PB4
thread1_loop:
    sbi PINB, PB4
    mov r1, r4
    rcall wait
    rjmp thread1_loop

    ;
    ; Thread 2: updates frequency for pin 3 LED blinking
    ;
thread2:
    dec r3
    ldi r16, 32
    mov r1, r16
    rcall wait
    rjmp thread2

    ;
    ; Thread 3: updates frequency for pin 4 LED blinking
    ;
thread3:
    dec r4
    ldi r16, 32
    mov r1, r16
    rcall wait
    rjmp thread3

;
; wait - waits ~160 * (255/r1) milliseconds
;
wait:
    push r0
    push r1
    clr r0
waitloop:
    dec r0
    brne waitloop
    dec r1
    brne waitloop
    pop r1
    pop r0
    ret
