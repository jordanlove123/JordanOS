ENTRY(_start)

SECTIONS
{
    /*
     * Load the kernel at 1 MiB — the standard location for protected-mode
     * kernels. GRUB places itself below 1 MiB and hands control to us here.
     */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)   /* multiboot header must be in the first 8 KiB of the ELF */
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
