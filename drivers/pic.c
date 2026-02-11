/**
 * pic.c - Programmable Interrupt Controller (8259 PIC)
 * NexusOS - mapowanie IRQ na wektory 32-47
 */

#include "kernel/types.h"
#include "kernel/port.h"
#include "drivers/pic.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define ICW1      0x11
#define ICW4      0x01

void pic_init(void)
{
    outb(PIC1_CMD, ICW1);
    outb(PIC2_CMD, ICW1);
    outb(PIC1_DATA, 32);
    outb(PIC2_DATA, 40);
    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);
    outb(PIC1_DATA, ICW4);
    outb(PIC2_DATA, ICW4);
    /* Odmaskuj IRQ 0 (timer), 1 (klawiatura), 12 (mysz) */
    outb(PIC1_DATA, 0xFC);  /* 11111100 - zezwól na 0, 1 */
    outb(PIC2_DATA, 0xEF);  /* 11101111 - zezwól na IRQ 12 (mysz) */
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_CMD, 0x20);
    outb(PIC1_CMD, 0x20);
}

