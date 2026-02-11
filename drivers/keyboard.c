/**
 * keyboard.c - Klawiatura PS/2
 * NexusOS - skan kody, bufor znaków
 */

#include "kernel/types.h"
#include "kernel/port.h"
#include "drivers/keyboard.h"

#define DATA_PORT 0x60
#define STATUS_PORT 0x64

#define KEYBUF_SIZE 256
static uint8_t keybuf[KEYBUF_SIZE];
static volatile uint32_t keybuf_head = 0;
static volatile uint32_t keybuf_tail = 0;

/* Prosta mapa scancode -> ASCII (US QWERTY, tylko podstawowe) */
static const char scancode_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void keyboard_init(void)
{
    (void)keybuf;
}

void keyboard_irq(void)
{
    uint8_t sc = inb(DATA_PORT);
    if (sc < sizeof(scancode_ascii) && scancode_ascii[sc]) {
        uint32_t next = (keybuf_head + 1) % KEYBUF_SIZE;
        if (next != keybuf_tail) {
            keybuf[keybuf_head] = scancode_ascii[sc];
            keybuf_head = next;
        }
    }
}

int keyboard_getchar(char* c)
{
    if (keybuf_tail == keybuf_head)
        return 0;
    *c = keybuf[keybuf_tail];
    keybuf_tail = (keybuf_tail + 1) % KEYBUF_SIZE;
    return 1;
}

uint32_t keyboard_available(void)
{
    return (keybuf_head - keybuf_tail + KEYBUF_SIZE) % KEYBUF_SIZE;
}
