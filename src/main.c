#include <stdint.h>
#include <util/delay.h>
#include "sysTime.h"

uint32_t lastTime = 0;
const uint32_t LED_DURATION = 500;
int main(void) {
  sei(); //! enables global interrupts
  sysTime_init();

  //! DDR (Data Direction Register) determines the DIRECTION of the data (Input or Output)
  const int8_t BUILTIN_LED = PB5; // built-in LED
  const int8_t PIN8_OUT = PB0; // pin 8
  DDRB |= _BV(BUILTIN_LED); 
  DDRB |= _BV(PIN8_OUT); // _BV equivalent to  1<<x

  while (1)
  {
    uint32_t currentTime = sysTime();
    //? if LED_DURATION has elapsed since last time
    if (currentTime - lastTime>= LED_DURATION){ 
      // 1. Toggle the LED
      PORTB ^= (1 << BUILTIN_LED);

      // 2. Read the new LED value
      uint8_t led_value = (PORTB >> BUILTIN_LED) & 1;

      // 3. Compute negated value (0 → 1, 1 → 0)
      uint8_t negated_value = !led_value;

      // 4. Clear the target bit before setting
      PORTB &= ~(1 << PIN8_OUT);

      // 5. Set the target bit to the negated value
      PORTB |= (negated_value << PIN8_OUT);

      // 6. Reset lastTime for the next interval
      lastTime = currentTime;
    }

    // do something
  }
  
}