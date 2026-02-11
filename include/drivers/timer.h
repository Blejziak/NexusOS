#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include "kernel/types.h"

void timer_init(void);
void timer_tick(void);
uint32_t timer_get_ticks(void);

#endif
