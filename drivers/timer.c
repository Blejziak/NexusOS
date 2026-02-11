/**
 * timer.c - Programmable Interval Timer (PIT)
 * NexusOS - IRQ0, ok. 100 Hz
 */

#include "kernel/types.h"
#include "kernel/port.h"
#include "drivers/timer.h"

#define PIT_FREQ    1193180
#define PIT_CH0     0x40
#define PIT_CMD     0x43

static volatile uint32_t ticks = 0;

void timer_init(void)
{
    uint32_t divisor = PIT_FREQ / 100;
    outb(PIT_CMD, 0x36);
    outb(PIT_CH0, divisor & 0xFF);
    outb(PIT_CH0, (divisor >> 8) & 0xFF);
}

void timer_tick(void)
{
    ticks++;
}

uint32_t timer_get_ticks(void)
{
    return ticks;
}
