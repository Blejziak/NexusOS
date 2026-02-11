#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include "kernel/types.h"

/* Stuby ISR - generowane w isr.asm */
extern void (*isr_stub_table[])(void);
extern void irq_0(void), irq_1(void), irq_2(void), irq_3(void);
extern void irq_4(void), irq_5(void), irq_6(void), irq_7(void);
extern void irq_8(void), irq_9(void), irq_10(void), irq_11(void);
extern void irq_12(void), irq_13(void), irq_14(void), irq_15(void);

void idt_init(void);
void idt_install_handler(uint8_t num, uint32_t handler);

#endif
