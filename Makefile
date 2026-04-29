CC      := x86_64-linux-gnu-gcc
AS      := x86_64-linux-gnu-as
LD      := x86_64-linux-gnu-ld

# GCC_INC: the compiler's own built-in headers (stdint.h, stddef.h, etc.)
# -nostdinc blocks system headers; -isystem adds back only these intrinsics.
GCC_INC := $(shell $(CC) -print-file-name=include)
CFLAGS  := -ffreestanding -nostdlib -nostdinc -isystem $(GCC_INC) -O2 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-avx

OBJ := obj

all: iso

run: iso
	qemu-system-x86_64 -cdrom myos.iso

assemble: boot/boot.s
	$(AS) boot/boot.s -o $(OBJ)/boot.o

assemble_interrupts: boot/interrupts.s
	$(AS) boot/interrupts.s -o $(OBJ)/interrupts.o

compile: kernel/kernel.c
	$(CC) -c kernel/kernel.c -o $(OBJ)/kernel.o $(CFLAGS)

compile_mem: kernel/memory.c
	$(CC) -c kernel/memory.c -o $(OBJ)/memory.o $(CFLAGS)

compile_pmm: kernel/pmm.c
	$(CC) -c kernel/pmm.c -o $(OBJ)/pmm.o $(CFLAGS)

compile_gdt: kernel/gdt.c
	$(CC) -c kernel/gdt.c -o $(OBJ)/gdt.o $(CFLAGS)

compile_vga: kernel/vga.c
	$(CC) -c kernel/vga.c -o $(OBJ)/vga.o $(CFLAGS)

compile_idt: kernel/idt.c
	$(CC) -c kernel/idt.c -o $(OBJ)/idt.o $(CFLAGS)

compile_pic: kernel/pic.c
	$(CC) -c kernel/pic.c -o $(OBJ)/pic.o $(CFLAGS)

compile_keyboard: kernel/keyboard.c
	$(CC) -c kernel/keyboard.c -o $(OBJ)/keyboard.o $(CFLAGS)

compile_paging: kernel/paging.c
	$(CC) -c kernel/paging.c -o $(OBJ)/paging.o $(CFLAGS)

compile_heap: kernel/heap.c
	$(CC) -c kernel/heap.c -o $(OBJ)/heap.o $(CFLAGS)

compile_fb: kernel/fb.c
	$(CC) -c kernel/fb.c -o $(OBJ)/fb.o $(CFLAGS)

compile_beep: kernel/beep.c
	$(CC) -c kernel/beep.c -o $(OBJ)/beep.o $(CFLAGS)

compile_pit: kernel/pit.c
	$(CC) -c kernel/pit.c -o $(OBJ)/pit.o $(CFLAGS)

compile_shell: kernel/shell.c
	$(CC) -c kernel/shell.c -o $(OBJ)/shell.o $(CFLAGS)

compile_string: kernel/string.c
	$(CC) -c kernel/string.c -o $(OBJ)/string.o $(CFLAGS)

link: assemble assemble_interrupts compile compile_mem compile_pmm compile_paging compile_fb compile_beep compile_idt compile_pic compile_keyboard compile_heap compile_pit compile_shell compile_string
	$(LD) $(OBJ)/boot.o $(OBJ)/interrupts.o $(OBJ)/memory.o $(OBJ)/pmm.o $(OBJ)/paging.o $(OBJ)/fb.o $(OBJ)/beep.o $(OBJ)/idt.o $(OBJ)/pic.o $(OBJ)/keyboard.o $(OBJ)/heap.o $(OBJ)/pit.o $(OBJ)/shell.o $(OBJ)/string.o $(OBJ)/kernel.o -o isodir/boot/myos.kernel -m elf_x86_64 -T linker.ld -nostdlib

iso: link
	grub-mkrescue -o myos.iso isodir

install: iso
	cp isodir/boot/myos.kernel /boot/myos.kernel

clean:
	rm -f $(OBJ)/*.o isodir/boot/myos.kernel myos.iso
