#include "tone.h"

//! use timer1 for tone generation because compare value is 16 bits
void tone_init(void) {
    //! Waveform generation mode set to CTC mode
    TCCR1B |= (1<<WGM12);

    //! Toggle OC1A on Compare Match
    TCCR1A |= (1 << COM1A0);

    //! Calling Interupts
    // ? OCR (Output Compare Register) (compare to timer counter value)
    // Set compare value for 440 Hz
    // OCR1A = 18181;
    // OCR1A = 20407;
    // OCR1A = 24241;
    

    //? TIMSK (Timer/Counter Interrupt Mask Register)
    //? OCIE (Output Compare Interrupt Enable) (this will enable the interrupt when OCR0A is true)
    // TIMSK1 |= (1<<OCIE1A); //! this without ISR will crash

    //! Prescalers (ticks by default 16Mhz) 
    // no prescaling
    TCCR1B |= (1<<CS10);
}

// ISR(TIMER1_COMPA_vect){
// }

void tone_off(){
    //! COM1A1 and COM1A0 form 2 bit fields (they work together, so to turn them off turn both off)
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); // disconnect timer from pin
    TCCR1B &= ~( (1<<CS12) | (1<<CS11) | (1<<CS10) ); // stop timer
    PORTB &= ~(1 << BUZZER_PIN); // force pin low
}

void tone_on(uint16_t ocr) {
    if (ocr == 0) {
        // pause: do not start timer
        tone_off();
    } else {
        OCR1A = ocr;
        TCCR1A |= (1 << COM1A0); // toggle on compare match
        TCCR1B |= (1 << CS10); // start timer with prescaler=1
    }
}

bool tone_is_on(void){
    return (TCCR1A & (1 << COM1A0)) != 0;
}

void tone_toggle(uint16_t ocr) {
    const bool tone_is_on = (TCCR1A & (1 << COM1A0)) != 0;
    if (tone_is_on) tone_off();
    else tone_on(ocr);
}

void set_tone(uint16_t ocr){
    OCR1A = ocr;
}

// uint16_t gen_compare_val(uint16_t target_freq){
//     const uint8_t N = 1; //* prescaler
//     //* F_CPU = 16Mhz
//     uint16_t compare_value = F_CPU/(2*N*target_freq) - 1;
//     return compare_value;
// }

// uint16_t gen_freq_val(uint16_t compare_val){
//     const uint8_t N = 1; // prescaler
//     uint16_t freq = F_CPU/(2*N*(compare_val + 1));
//     return freq;
// }