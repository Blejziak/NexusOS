/**
 * kernel.c - Punkt wejścia kernela
 * NexusOS 0.1.0
 */

#include "kernel/types.h"
#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/multiboot.h"
#include "drivers/pic.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"
#include "drivers/framebuffer.h"
#include "gui/desktop.h"

/* Zmienne z boot.asm - adres informacji Multiboot */
uint32_t multiboot_magic;
uint32_t multiboot_info;

static void banner(void)
{
    /* Logo / banner przy starcie - wyświetlane w GUI */
    (void)0;
}

void kernel_main(uint32_t magic, uint32_t mb_info_phys)
{
    (void)magic;
    multiboot_magic = magic;
    multiboot_info  = mb_info_phys;

    gdt_init();
    idt_init();
    pic_init();
    timer_init();
    keyboard_init();
    mouse_init();
    fb_init((void*)(uint32_t)mb_info_phys);

    __asm__ volatile("sti");

    banner();
    gui_desktop_init();
    gui_desktop_run();
}
