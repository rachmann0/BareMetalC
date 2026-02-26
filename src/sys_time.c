#include "sys_time.h"

volatile static uint32_t ms_counter = 0; 
// volatile tells compiler to NOT optimize this variable / “This variable can change outside normal program flow.”
// and static so that no external file can change this variable

void sys_time_init(void) {
    /*
     The counter value (TCNT0) increases until a compare match
     occurs between TCNT0 and OCR0A, and then counter (TCNT0) is cleared.
    */

    //? TCCR (Timer/Counter Control Register)

    //! Waveform settings (Waveform generation mode)
    //? Waveform generation mode set to CTC mode
    TCCR0A |= (1<<WGM01);
    TCCR0A &= ~(1<<WGM00);
    TCCR0B &= ~(1<<WGM02);

    //! Calling Interupts
    //? OCR (Output Compare Register) (compare to timer counter value)
    OCR0A = 249; // counts from 0 and we want 250 ticks

    //? TIMSK (Timer/Counter Interrupt Mask Register)
    //? OCIE (Output Compare Interrupt Enable) (this will enable the interrupt when OCR0A is true)
    TIMSK0 |= (1<<OCIE0A);

    //! Prescalers (ticks by default 16Mhz)
    //? set prescaler bits last as it is what PHYSICALLY starts the timer
    //* 16 000 000 / 64 = 250 000 / 1000ms => 250ticks/1ms
    //? prescaler set to 64
    TCCR0B |= (1<<CS01);
    TCCR0B |= (1<<CS00);
    TCCR0B &= ~(1<<CS02);
}

ISR(TIMER0_COMPA_vect){
    ms_counter++;
}

uint32_t sys_time(void){
    //* we do this stop and start again to ensure the ms counter is not changing in value when its assigned
    //* this is called atomic section or critical section
    //* “Atomic” means: Indivisible — cannot be interrupted halfway through.

    uint32_t time;
    cli(); //! stop all interrupts
    time = ms_counter; //* ISR cannot run in between this (this is the Atomic Section)
    sei(); //! enables global interrupts
    return time;

    //* you don't need this logic when the variable is say 8 bits (e.g. uint8_t)
    //* Because 8-bit read is already atomic on AVR.

    //* you need this for larger bits (e.g. uint32_t) because
    //* Reading that variable takes 4 separate memory accesses on an 8-bit AVR.
    //* Interrupt might fire in between that 4 separate memory access.
}