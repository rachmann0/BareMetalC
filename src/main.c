#include <stdint.h>
#include <util/delay.h>

#include <avr/pgmspace.h>

#include "sys_time.h"
#include "tone.h"
#include "config.h"
#include "music.h"


uint32_t last_time = 0;
uint32_t last_time_note = 0;
const uint32_t LED_DURATION = 500;

const Note melody[] PROGMEM = { //! PROGMEM stores it in flash (not SRAM)
    {GET_COMPARE_VAL(440), 500},  // A4, 500ms
    {GET_COMPARE_VAL(392), 500},  // G4, 500ms
    {GET_COMPARE_VAL(330), 500},  // E4, 500ms
    {GET_COMPARE_VAL(262), 500}   // C4, 500ms
};
const uint8_t melody_length = sizeof(melody)/sizeof(melody[0]);

int main(void) {
  sys_time_init();
  tone_init();
  sei(); //! enables global interrupts once after both timer inits

  //! DDR (Data Direction Register) determines the DIRECTION of the data (Input or Output)
  DDRB |= _BV(BUILTIN_LED); // _BV equivalent to  1<<x
  DDRB |= _BV(EXTERNAL_LED); 
  DDRB |= _BV(BUZZER_PIN);

  size_t melody_index = 0;
  uint16_t current_pitch = pgm_read_word(&melody[melody_index].ocr);
  uint16_t current_note_duration = pgm_read_word(&melody[melody_index].duration_ms);
  set_tone(current_pitch);
  while (1)
  {
    uint32_t current_time = sys_time();

    //? if current_note_duration has elapsed since last time, change notes
    if (current_time - last_time_note >= current_note_duration)
    {
      if (melody_index < melody_length) {
        // turn off current melody first
        tone_off();
        // increment melody
        melody_index++;
        current_pitch = pgm_read_word(&melody[melody_index].ocr);
        current_note_duration = pgm_read_word(&melody[melody_index].duration_ms);
        // turn on next melody
        tone_on(current_pitch);

        last_time_note += current_note_duration;
      } else {
        tone_off();
      }
      
    }

    //? if LED_DURATION has elapsed since last time, toggle LED
    if (current_time - last_time >= LED_DURATION){ 
      // 1. Toggle the LED
      PORTB ^= (1 << BUILTIN_LED);
      // 2. Read the new LED value
      uint8_t led_value = (PORTB >> BUILTIN_LED) & 1;
      // 3. Compute negated value (0 → 1, 1 → 0)
      uint8_t negated_value = !led_value;
      // 4. Clear the target bit before setting
      PORTB &= ~(1 << EXTERNAL_LED);
      // 5. Set the target bit to the negated value
      PORTB |= (negated_value << EXTERNAL_LED);
      // 6. Reset last_time for the next interval
      // last_time = current_time; //! this can introduce timer drift if loop is late
      last_time += LED_DURATION; //! do this instead
    }

    // do something
  }
  
}