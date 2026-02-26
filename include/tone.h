#ifndef TONE_H
#define TONE_H


#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"

//? init
void tone_init(void);

//? on-off
void tone_off();
void tone_on(uint16_t ocr);
bool tone_is_on(void);
void tone_toggle(uint16_t ocr);

//? set
void set_tone(uint16_t ocr);

//? utils
// uint16_t gen_compare_val(uint16_t target_freq);
// uint16_t gen_freq_val(uint16_t compare_val);

#define GET_COMPARE_VAL(target_freq) F_CPU/(2*target_freq) - 1
//* N = Prescaler
//* F_CPU = 16Mhz
/*
*   why the 2?
*   Because the pin toggles on every compare match.
*   A full square wave cycle = 2 toggles (HIGH → LOW → HIGH).
*/
#define GET_FREQ_VAL(compare_val) F_CPU/(2*(compare_val + 1))



#endif