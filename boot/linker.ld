/* linker.ld - Skrypt linkera dla NexusOS (x86) */
/* Kernel 32-bit dla uproszczenia z Multiboot2 - GRUB ładuje w protected mode */

ENTRY(_start)

SECTIONS
{
    . = 1M;

    .multiboot ALIGN(4K) : AT(ADDR(.multiboot))
    {
        *(.multiboot)
    }

    .text ALIGN(4K) : AT(ADDR(.text))
    {
        *(.text .text.*)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata))
    {
        *(.rodata .rodata.*)
    }

    .data ALIGN(4K) : AT(ADDR(.data))
    {
        *(.data .data.*)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss))
    {
        *(COMMON)
        *(.bss .bss.*)
        . = ALIGN(4K);
        _kernel_end = .;
    }

    _kernel_end_symbol = _kernel_end;
}
