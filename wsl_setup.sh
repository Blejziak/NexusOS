#!/bin/bash
# Pełna konfiguracja i budowa NexusOS w WSL
# Uruchom: wsl bash wsl_setup.sh  (z poziomu Windows w katalogu System)
# lub w WSL: ./wsl_setup.sh

set -e
echo "=== Instalacja zależności (WSL/Debian/Ubuntu) ==="
sudo apt update
sudo apt install -y build-essential nasm grub-pc-bin grub2-common gcc-i686-elf qemu-system-x86

echo ""
echo "=== Budowanie kernela ==="
make clean 2>/dev/null || true
make all

echo ""
echo "=== Tworzenie ISO ==="
make iso

echo ""
echo "=== Gotowe. Uruchamianie QEMU... ==="
qemu-system-i386 -cdrom nexusos.iso -m 128 -no-reboot -no-shutdown
