# When GRUB tries to load the OS, it looks for this in the first 8 KB of
# memory. First, it looks for the magic number which tells it where the
# setup starts. In the setup, we reserve space for a stack so we can use
# C, and then call the kernel script which runs the functions of the OS

.set MB2_MAGIC, 0xE85250D6
.set MB2_ARCH,  0

# Multiboot 2 header
.section .multiboot
.align 8
mb2_header_start:
.long MB2_MAGIC
.long MB2_ARCH
.long mb2_header_end - mb2_header_start
.long -(MB2_MAGIC + MB2_ARCH + (mb2_header_end - mb2_header_start))

# Framebuffer tag (type 5)
.align 8
.word 5         # type
.word 0         # flags
.long 20        # size
.long 0         # preferred width  (0 = don't care)
.long 0         # preferred height
.long 0         # preferred depth

# End tag (type 0)
.align 8
.word 0         # type
.word 0         # flags
.long 8         # size
mb2_header_end:

# Reserve 16 KB stack space at .bss
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

# bootstrap page tables
.global pml4
.global pdpt
.global fb_pd
.align 4096
pml4:
.skip 4096
pdpt:
.skip 4096
pd:
.skip 4096
fb_pd:
.skip 4096

.section .data
gdt64:
.quad 0
.quad 0x00AF9A000000FFFF
.quad 0x00CF92000000FFFF
gdt64_end:

gdt64_ptr:
.word gdt64_end - gdt64 - 1
.long gdt64

# Call kernel
.text
.global _start
_start:
.code32
    movl  $stack_top, %esp
    movl  %eax, %edi            # save kernel args
    movl  %ebx, %esi

    # set up bootstrap page table
    movl  $pdpt, %eax
    orl   $0x3, %eax             # load pdpt[0] address + flags to eax
    movl  %eax, pml4
    movl  $pd, %eax
    orl   $0x3, %eax             # load pd[0] address + flags to eax
    movl  %eax, pdpt
    # identity map first 1 GB of memory in pd for kernel
    movl  $0, %eax              # index in eax
l1:
    cmp   $512, %eax
    jge   l1end
    movl  %eax, %ebx
    sall  $21, %ebx
    orl   $0x83, %ebx
    movl  %ebx, pd(, %eax, 8)
    incl  %eax
    jmp   l1
l1end:
    # set bit 5 of CR4 (enable PAE)
    movl  %cr4, %eax
    orl   $0x20, %eax
    movl  %eax, %cr4
    movl  $pml4, %eax
    movl  %eax, %cr3
    # set bit 8 of IA32_EFER (long mode enable)
    movl  $0xC0000080, %ecx
    rdmsr
    orl   $0x100, %eax
    wrmsr
    # set bit 31 of CR0 to activate long mode
    movl  %cr0, %eax
    orl   $0x80000000, %eax
    movl  %eax, %cr0
    # Load GDT data
    lgdt  gdt64_ptr
    ljmp  $0x08, $.flush
.flush:
.code64
    mov   $0x10, %ax
    mov   %ax, %ds
    mov   %ax, %es
    mov   %ax, %fs
    mov   %ax, %gs
    mov   %ax, %ss
    mov   $stack_top, %rsp      # set up stack
    
    # start kernel
    call kernel_main
# if next line is reached then kernel exited. this shouldn't happen so
# just stop everything and disable interrupts
    cli
1:  hlt
    jmp  1b
