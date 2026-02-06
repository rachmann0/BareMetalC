#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void sysTime_init(void) {
    //Waveform settings
    // TCCR0A |= (1<<WGM01)
    // TCCR0A &= ~(1<<WGM00)

    //Prescalers
    //Calling Interupts
}