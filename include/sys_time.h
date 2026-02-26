#ifndef sys_time_H
#define sys_time_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void sys_time_init(void);
uint32_t sys_time(void);


#endif