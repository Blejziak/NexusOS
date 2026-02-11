# Makefile - NexusOS
# Wymaga: i686-elf-gcc (lub gcc -m32), NASM, grub-mkrescue

CC       = i686-elf-gcc
ASM      = nasm
LD       = i686-elf-ld
CFLAGS   = -ffreestanding -fno-stack-protector -fno-pie -Wall -Wextra -I include -m32 -O2
ASMFLAGS = -f elf32
LDFLAGS  = -T boot/linker.ld -m elf_i386 -nostdlib

# Jeśli nie masz i686-elf-gcc, użyj zwykłego gcc (działa na wielu systemach):
# CC = gcc
# LD = ld
# Wtedy dodaj: LDFLAGS += -m elf_i386  (dla ld)

KERNEL   = nexusos.elf
ISO      = nexusos.iso
ISO_DIR  = isodir

BOOT_ASM = boot/multiboot.asm boot/gdt_flush.asm boot/idt_load.asm boot/isr.asm
BOOT_O   = $(BOOT_ASM:.asm=.o)

KERNEL_C = kernel/kernel.c kernel/gdt.c kernel/idt.c kernel/isr.c
DRIVERS_C = drivers/pic.c drivers/timer.c drivers/keyboard.c drivers/mouse.c drivers/framebuffer.c
GUI_C    = gui/desktop.c gui/theme.c gui/window.c
APPS_C   = apps/settings.c
SHELL_C  = shell/shell.c

SRCS     = $(KERNEL_C) $(DRIVERS_C) $(GUI_C) $(APPS_C) $(SHELL_C)
OBJS     = $(BOOT_O) $(SRCS:.c=.o)

.PHONY: all clean iso run

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(KERNEL) $(ISO)
	rm -rf $(ISO_DIR)

# Katalog na ISO: GRUB wymaga boot/grub/grub.cfg i boot/kernel
iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp boot/grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(ISO) $(ISO_DIR)
	@echo "Utworzono $(ISO) - uruchom w QEMU: make run"

run: iso
	qemu-system-i386 -cdrom $(ISO) -m 128 -no-reboot -no-shutdown

run-vga: iso
	qemu-system-i386 -cdrom $(ISO) -m 128 -vga std -no-reboot -no-shutdown
