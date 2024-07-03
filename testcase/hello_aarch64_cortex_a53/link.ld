OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_Reset)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x40080000, LENGTH = 1M
    RAM (rwx) : ORIGIN = 0x50000000, LENGTH = 32M
}

SECTIONS
{
    .text : {
        *(.vector_table)
        *(.text*)
        *(.rodata*)
        *(.ARM.exidx)
        . = ALIGN(16);
     } > ROM
     __text_end = .;

    .data : AT(__text_end)
    {
        __data_start = .;
        *(.data)
        . = ALIGN(16);
        __data_end = .;
    } > RAM
    .bss : {
        __bss_start = .;
        *(.bss)
        . = ALIGN(16);
        __bss_end = .;
    } > RAM

    _stack_start = .;
    _stack_end = _stack_start + 0x1000; /* 4 KB */
}
