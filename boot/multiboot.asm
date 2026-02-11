; multiboot.asm - Nagłówek Multiboot2 dla GRUB2
; NexusOS - boot

MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
VIDEO    equ  1 << 2
FLAGS    equ  MBALIGN | MEMINFO | VIDEO
MAGIC    equ  0xE85250D6
; Długość nagłówka: 16 (magic+arch+len+checksum) + 20 (framebuffer tag) + 8 (end tag) = 44, align 8 = 48
HEADER_LEN equ 48
CHECKSUM equ -(MAGIC + 0 + HEADER_LEN)

section .multiboot
align 8
multiboot_header:
    dd MAGIC
    dd 0
    dd HEADER_LEN
    dd CHECKSUM
    ; Tag: Framebuffer (type 5, size 20) - 0,0,0 = domyślna rozdzielczość
    dw 5
    dw 0
    dd 20
    dd 0
    dd 0
    dd 0
    ; Tag: Koniec
    dw 0
    dw 0
    dd 8

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main
extern multiboot_info
extern multiboot_magic

_start:
    mov esp, stack_top
    mov [multiboot_magic], eax
    mov [multiboot_info], ebx
    push ebx
    push eax
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang
