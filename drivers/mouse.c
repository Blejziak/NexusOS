/**
 * mouse.c - Mysz PS/2
 * NexusOS - pakiet 3 bajty, ruch i przyciski
 */

#include "kernel/types.h"
#include "kernel/port.h"
#include "drivers/mouse.h"

#define DATA_PORT 0x60
#define STATUS_PORT 0x64
#define CMD_PORT 0x64

#define MOUSE_CMD 0xD4
#define MOUSE_ENABLE 0xF4

static int mouse_phase = 0;
static uint8_t mouse_byte[3];
static int16_t mouse_x = 0, mouse_y = 0;
static uint8_t mouse_buttons = 0;
static volatile int mouse_updated = 0;

void mouse_init(void)
{
    outb(CMD_PORT, MOUSE_CMD);
    outb(DATA_PORT, MOUSE_ENABLE);
    mouse_phase = 0;
}

void mouse_irq(void)
{
    uint8_t d = inb(DATA_PORT);
    switch (mouse_phase) {
        case 0:
            if (d & 0x08) {
                mouse_byte[0] = d;
                mouse_phase = 1;
            }
            break;
        case 1:
            mouse_byte[1] = d;
            mouse_phase = 2;
            break;
        case 2:
            mouse_byte[2] = d;
            mouse_buttons = mouse_byte[0] & 0x07;
            mouse_x += (int16_t)(mouse_byte[0] & 0x10 ? (mouse_byte[1] | 0xFF00) : mouse_byte[1]);
            mouse_y -= (int16_t)(mouse_byte[0] & 0x20 ? (mouse_byte[2] | 0xFF00) : mouse_byte[2]);
            mouse_updated = 1;
            mouse_phase = 0;
            break;
    }
}

void mouse_get(int16_t* x, int16_t* y, uint8_t* buttons)
{
    *x = mouse_x;
    *y = mouse_y;
    *buttons = mouse_buttons;
}

int mouse_has_update(void)
{
    return mouse_updated;
}

void mouse_clear_update(void)
{
    mouse_updated = 0;
}

void mouse_set_position(int16_t x, int16_t y)
{
    mouse_x = x;
    mouse_y = y;
}
