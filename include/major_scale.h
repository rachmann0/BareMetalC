#ifndef MAJOR_SCALE_H
#define MAJOR_SCALE_H

#include "tone.h"
#include "music.h"
#include <avr/pgmspace.h>


const Note melody[] PROGMEM = { //! PROGMEM stores it in flash (not SRAM)
    {GET_COMPARE_VAL(262), 400},  // C4
    {GET_COMPARE_VAL(294), 400},  // D4
    {GET_COMPARE_VAL(330), 400},  // E4
    {GET_COMPARE_VAL(349), 400},  // F4
    {GET_COMPARE_VAL(392), 400},  // G4
    {GET_COMPARE_VAL(440), 400},  // A4
    {GET_COMPARE_VAL(494), 400},  // B4
    {GET_COMPARE_VAL(523), 600},   // C5

    {0, 300},  // pause

    {GET_COMPARE_VAL(523), 600},   // C5
    {GET_COMPARE_VAL(494), 400},  // B4
    {GET_COMPARE_VAL(440), 400},  // A4
    {GET_COMPARE_VAL(392), 400},  // G4
    {GET_COMPARE_VAL(349), 400},  // F4
    {GET_COMPARE_VAL(330), 400},  // E4
    {GET_COMPARE_VAL(294), 400},  // D4
    {GET_COMPARE_VAL(262), 400},  // C4
};
const uint8_t melody_length = sizeof(melody)/sizeof(melody[0]);


#endif