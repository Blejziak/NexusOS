#ifndef DRIVERS_MOUSE_H
#define DRIVERS_MOUSE_H

#include "kernel/types.h"

void mouse_init(void);
void mouse_irq(void);
void mouse_get(int16_t* x, int16_t* y, uint8_t* buttons);
int mouse_has_update(void);
void mouse_clear_update(void);
void mouse_set_position(int16_t x, int16_t y);

#endif
