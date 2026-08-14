


ENTRY(_start)

SECTIONS
{
    /* Multiboot butuh kernel di-load di alamat 1MB ke atas */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text.boot)   /* Pastikan multiboot header di paling depan! */
        *(.text*)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata*)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data*)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss*)
    }
}