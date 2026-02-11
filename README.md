# NexusOS

Prosty system operacyjny inspirowany Linuxem, napisany w C (kernel) z warstwą GUI.  
Architektura: **x86 (32-bit)**, boot: **GRUB2 (Multiboot2)**.

**Nazwa systemu:** NexusOS (możesz zmienić w `include/kernel/types.h`: `OS_NAME`, `OS_VERSION`)

---

## Szybki start (WSL)

1. Otwórz **WSL** (Ubuntu/Debian) – z menu Start wpisz „Ubuntu” lub „wsl”.
2. Wklej i wykonaj (instalacja, build, ISO, QEMU):

```bash
sudo apt install build-essential nasm grub-pc-bin grub2-common gcc-i686-elf qemu-system-x86
cd /mnt/c/Users/xxxco/Desktop/System
make all
make iso
qemu-system-i386 -cdrom nexusos.iso -m 128
```

**Alternatywa – jeden skrypt** (w katalogu projektu w WSL):

```bash
cd /mnt/c/Users/xxxco/Desktop/System
chmod +x wsl_setup.sh
./wsl_setup.sh
```

**Build w chmurze (bez WSL na PC):**  
Wypchnij repo na GitHub. W **Actions** uruchomi się workflow „Build NexusOS”; po zakończeniu pobierz **nexusos.iso** z Artifacts. Uruchom go w QEMU lub VirtualBox.

---

## Wymagania

- **Kompilator:** `i686-elf-gcc` (cross-compiler) lub `gcc` z opcją `-m32`
- **Asembler:** NASM
- **Linker:** `i686-elf-ld` lub `ld` (elf_i386)
- **ISO:** `grub-mkrescue` (część GRUB2)
- **Testy:** QEMU (`qemu-system-i386`) lub VirtualBox

### Instalacja cross-compilera (Linux)

```bash
# Debian/Ubuntu
sudo apt install gcc-multilib nasm grub-pc-bin grub2-common
# Cross-compiler (opcjonalnie, zalecane)
sudo apt install gcc-i686-elf  # lub zbuduj z source: osdev.org/wiki/GCC_Cross-Compiler
```

### Windows

- **Make:** Zainstaluj [MinGW-w64](https://www.mingw-w64.org/) / [MSYS2](https://www.msys2.org/) (z pakietem `make`) lub użyj **WSL** (Windows Subsystem for Linux) i kompiluj w Linuxie.
- **Kompilator:** `i686-elf-gcc` (cross-compiler, np. z [osdev.org](https://wiki.osdev.org/GCC_Cross-Compiler)) lub w WSL: `sudo apt install gcc-i686-elf`.
- **NASM:** [nasm.us](https://www.nasm.us/).
- **GRUB2 / grub-mkrescue:** Zazwyczaj dostępne w WSL lub w dystrybucji Linux w MSYS2.
- **QEMU:** [qemu.org](https://www.qemu.org/download/) – do testów.

---

## Struktura projektu

```
System/
├── boot/           # Multiboot2, GDT flush, IDT load, ISR (asm)
├── kernel/         # kernel.c, GDT, IDT, ISR (C)
├── drivers/        # PIC, timer, keyboard, mouse, framebuffer
├── fs/             # (VFS – do rozbudowy)
├── gui/            # desktop, taskbar, dock, okna, motywy
├── shell/          # powłoka (aplikacja)
├── apps/           # ustawienia, shell
├── include/        # nagłówki (kernel/, drivers/, gui/, apps/)
├── Makefile
├── build_iso.sh    # skrypt budowania ISO (Linux)
├── build_iso.bat   # skrypt budowania ISO (Windows)
├── wsl_setup.sh    # pełna konfiguracja WSL: apt, make, iso, qemu
└── README.md
```

---

## Kompilacja

### Wszystko (kernel + ISO)

```bash
make all
make iso
```

Lub jednym poleceniem (Linux):

```bash
./build_iso.sh
```

Na Windows (jeśli masz `make` i narzędzia):

```bat
build_iso.bat
```

### Tylko kernel

```bash
make all
```

Plik wynikowy: **nexusos.elf**.

### Tworzenie ISO

```bash
make iso
```

Wymaga: `grub-mkrescue`. Katalog `isodir/` musi zawierać:

- `isodir/boot/nexusos.elf` – kernel
- `isodir/boot/grub/grub.cfg` – konfiguracja GRUB (kopiowane przez Makefile)

---

## Uruchomienie

### QEMU

```bash
make run
```

lub:

```bash
qemu-system-i386 -cdrom nexusos.iso -m 128 -no-reboot -no-shutdown
```

Z lepszą obsługą VGA:

```bash
qemu-system-i386 -cdrom nexusos.iso -m 128 -vga std
```

### VirtualBox

1. Nowa maszyna → typ: Other, wersja: Other/Unknown (32-bit).
2. Pamięć RAM: np. 128 MB.
3. Dysk: wybierz istniejący dysk → **nexusos.iso**.
4. Uruchom.

---

## Funkcje

- **Kernel:** GDT, IDT, PIC, przerwania (timer, klawiatura, mysz PS/2).
- **Framebuffer:** Linear framebuffer z Multiboot2 (VESA).
- **GUI:** pulpit, górny pasek (taskbar), dock na środku, ikona ustawień (lewo), zegar (prawo).
- **Okna:** prosty menedżer okien, pasek tytułowy, przycisk zamknięcia.
- **Ustawienia:** okno z informacjami o systemie, zmiana motywu, placeholder dla RAM, wyłączenie/restart.
- **Shell:** aplikacja w docku (powłoka z komendami: help, ls, cd, clear, echo, reboot, shutdown).
- **Motywy:** kilka zestawów kolorów (pasek, dock, akcent).
- **Boot:** logo przy starcie (prosty ekran powitalny).

---

## Wersjonowanie

Aktualna wersja: **0.1.0** (w `include/kernel/types.h`: `OS_VERSION`).

---

## Licencja

Projekt edukacyjny – możesz używać i modyfikować według potrzeb.
