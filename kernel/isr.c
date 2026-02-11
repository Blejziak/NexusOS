/**
 * isr.c - Handlery przerwań (ISR/IRQ)
 * NexusOS
 */

#include "kernel/types.h"
#include "kernel/idt.h"
#include "drivers/pic.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"

void isr_handler(uint32_t* regs)
{
    (void)regs;
    /* Tutaj obsługa wyjątków CPU - na razie ignorujemy */
}

void irq_handler(uint32_t* regs)
{
    /* Na stosie: pusha(8), ds,es,fs,gs(4), ret(1) -> int_no to regs[13] */
    uint32_t int_no = regs[13];
    if (int_no >= 32 && int_no <= 47) {
        if (int_no == 32)
            timer_tick();
        else if (int_no == 33)
            keyboard_irq();
        else if (int_no == 44)
            mouse_irq();
        pic_send_eoi(int_no - 32);
    }
}
