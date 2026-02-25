#ifndef SYSTIME_H
#define SYSTIME_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void sysTime_init(void);
uint32_t sysTime(void);


#endif