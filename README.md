# JordanOS

A hobby operating system written in C and x86-64 assembly, booted via GRUB with the Multiboot2 protocol. Runs in 64-bit long mode with a framebuffer terminal interface.

## Features

- 64-bit long mode kernel
- Framebuffer text rendering with bitmap font and scaling
- Physical memory manager
- Heap allocator (kmalloc/kfree)
- IDT with CPU exception handlers
- 8259 PIC with keyboard and timer support
- PS/2 keyboard driver with scancode mapping
- PIT timer at 100 Hz with blinking cursor
- Interactive terminal with theming support

## Terminal Commands

| Command | Description |
|---------|-------------|
| `help` | List available commands |
| `clear` | Clear the screen |
| `mem` | Show memory statistics |
| `theme <name>` | Change color theme (`theme --help` for options) |
| `reboot` | Reboot the computer |

## Building

### Dependencies

On Debian/Ubuntu:
```
sudo apt install gcc-x86-64-linux-gnu binutils-x86-64-linux-gnu grub-pc-bin grub-common xorriso mtools qemu-system-x86
```

On Arch Linux:
```
sudo pacman -S x86_64-linux-gnu-gcc x86_64-linux-gnu-binutils grub xorriso mtools qemu-system-x86
```

### Build

```
make
```

This produces `myos.iso`.

## Running

### QEMU (recommended for testing)

```
make run
```

Or manually:
```
qemu-system-x86_64 -cdrom myos.iso
```

To allocate more RAM (default is 128 MB):
```
qemu-system-x86_64 -cdrom myos.iso -m 512M
```

### Real Hardware via GRUB

> **Warning:** This will modify your GRUB configuration. Only do this if you are comfortable with bootloader setup and have a way to recover if something goes wrong (e.g. a live USB).

> **Secure Boot:** JordanOS is an unsigned kernel and will not boot on systems with Secure Boot enabled. You will need to disable Secure Boot in your UEFI firmware settings before it will load. Disabling Secure Boot reduces a layer of protection against unsigned bootloaders and operating systems — do this at your own risk and re-enable it if you remove JordanOS.

1. Build the project to produce `myos.iso`, then copy the kernel binary to your boot partition:
```
sudo make install
```
This copies `myos.kernel` to `/boot/myos.kernel`.

2. Add an entry to your GRUB configuration. Edit `/etc/grub.d/40_custom` and add:
```
menuentry 'JordanOS' {
    insmod multiboot2
    set gfxpayload=keep
    multiboot2 /boot/myos.kernel
}
```

3. Update GRUB:
```
sudo update-grub       # Debian/Ubuntu
sudo grub-mkconfig -o /boot/grub/grub.cfg   # Arch/others
```

4. Reboot and select **JordanOS** from the GRUB menu.

To remove it, delete the entry from `/etc/grub.d/40_custom` and run `update-grub` again.

## Project Structure

```
boot/
  boot.s          - Multiboot2 header, page table setup, long mode entry
  interrupts.s    - ISR and IRQ stubs
include/          - Header files
kernel/
  kernel.c        - Entry point and initialization
  fb.c            - Framebuffer driver and terminal
  idt.c           - Interrupt descriptor table
  pic.c           - Programmable interrupt controller
  pit.c           - Programmable interval timer
  pmm.c           - Physical memory manager
  heap.c          - Heap allocator
  paging.c        - Page table management
  keyboard.c      - PS/2 keyboard driver
  shell.c         - Terminal shell
  string.c        - String utilities
  memory.c        - Multiboot2 memory map parsing
linker.ld         - Linker script (kernel loaded at 1 MB)
Makefile
```
