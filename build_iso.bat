@echo off
REM build_iso.bat - Budowanie NexusOS (Windows)
REM Wymaga: MinGW z i686-elf-gcc LUB WSL/Cygwin z make, nasm, grub-mkrescue
REM Dla samej kompilacji: make (jeśli masz make i cross-compiler)

echo === Budowanie NexusOS ===
make clean 2>nul
make all
if errorlevel 1 (
    echo Blad kompilacji. Zainstaluj: i686-elf-gcc, nasm, make.
    echo Zobacz README.md
    exit /b 1
)
echo === Tworzenie ISO ===
make iso
if errorlevel 1 (
    echo Brak grub-mkrescue. Zainstaluj GRUB2 lub zbuduj ISO w Linuxie.
    exit /b 1
)
echo === Gotowe: nexusos.iso ===
echo Uruchom: qemu-system-i386 -cdrom nexusos.iso -m 128
