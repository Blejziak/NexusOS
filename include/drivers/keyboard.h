#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

#include "kernel/types.h"

void keyboard_init(void);
void keyboard_irq(void);
int keyboard_getchar(char* c);
uint32_t keyboard_available(void);

#endif
