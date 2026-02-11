#!/bin/bash
# build_iso.sh - Budowanie kernela i obrazu ISO (NexusOS)
# Użycie: ./build_iso.sh
# Wymaga: make, grub-mkrescue (grub2-common), nasm, i686-elf-gcc (lub standard gcc)

set -e
echo "=== Budowanie NexusOS ==="
make clean 2>/dev/null || true
make all
echo "=== Tworzenie ISO ==="
make iso
echo "=== Gotowe: nexusos.iso ==="
echo "Uruchom: qemu-system-i386 -cdrom nexusos.iso -m 128"
echo "lub: make run"
