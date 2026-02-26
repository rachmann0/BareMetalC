//! NOTES AT THE BOTTOM !
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

const int8_t BUILTIN_LED = PB5; // built-in LED

ISR(TIMER1_OVF_vect){
  PORTB ^= _BV(BUILTIN_LED);
  TCNT1 = 65536 - (F_CPU/1024);
}

int main(void) {

  //! DDR (Data Direction Register) determines the DIRECTION of the data (Input or Output)
  DDRB |= _BV(BUILTIN_LED); 

  //! set T counter default value
  //? this has max value of 65535 (2^16) (16 bits)
  //? when it gets to 65536 and overflows it will trigger the ISR
  // TCNT1 = 0;

  //! if we want one second we need to set default value to (max T counter + 1) - (effective clock rate/prescaler)
  //* time = ticks / tick_frequency
  //* = 15625 / 15625Hz
  //* = 1 second
  //? this is called timer preloading
  TCNT1 = 65536 - (F_CPU/1024);

  //! Set prescaler to 1024
  //? instead of the counter ticking every 16Mhz it ticks every 16Mhz/1024
  //? 16Mhz/1024 = 15625Hz
  TCCR1B = (1<<CS10) | (1<<CS12);

  //! disable all other features. we only want to use overflow timer (Normal Mode in AVR)
  TCCR1A = 0x00;

  //! this will enable overflow interrupt
  TIMSK1 = (1 << TOIE1);

  //! enables global interrupts 
  sei();


  while (1)
  {
  }
  
}

/*
!Core Tradeoff for Prescales choice.
!Prescaler choice is always:
  Resolution vs Maximum Time
*Smaller prescaler:
  -Higher resolution
  -Smaller max delay before overflow
*Larger prescaler:
  -Lower resolution
  -Bigger possible delay

You can’t maximize both.
*/