/**
 * idt.c - Interrupt Descriptor Table (IDT)
 * NexusOS - obsługa przerwań
 */

#include "kernel/types.h"
#include "kernel/idt.h"

#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

extern void idt_load(uint32_t);

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

void idt_init(void)
{
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base  = (uint32_t)&idt;
    /* Wypełnij bramki - handlery zdefiniowane w isr.asm */
    /* 0x8E = present, ring 0, 32-bit interrupt gate */
    {
        uint32_t i;
        for (i = 0; i < 32; i++)
            idt_set_gate((uint8_t)i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }
    /* IRQ 0-15 mapowane na 32-47 */
    idt_set_gate(32, (uint32_t)irq_0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq_1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq_2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq_3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq_4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq_5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq_6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq_7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq_8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq_9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq_10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq_11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq_12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq_13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq_14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq_15, 0x08, 0x8E);
    idt_load((uint32_t)&idtp);
}

void idt_install_handler(uint8_t num, uint32_t handler)
{
    idt_set_gate(num, handler, 0x08, 0x8E);
}
