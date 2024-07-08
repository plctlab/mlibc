OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_Reset)

MEMORY
{
    ROM (rx) : ORIGIN = 0x08000000, LENGTH = 1M
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 32M
}

SECTIONS
{
    .text : {
        *(.vector_table)
        *(.text*)
        *(.rodata*)
        *(.ARM.exidx)
        . = ALIGN(4);
        __text_end = .;
     } > ROM

    .data : AT(__text_end)
    {
        __data_start = .;
        *(.data)
        . = ALIGN(4);
        __data_end = .;
    } > RAM
    .bss : {
        __bss_start = .;
        *(.bss)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    _fiq_stack_start = ADDR(.bss) + SIZEOF(.bss);
    _fiq_stack_end = _fiq_stack_start + 0x1000; /* 64 KB */

    _irq_stack_start = _fiq_stack_end;
    _irq_stack_end = _irq_stack_start + 0x1000; /* 64 KB */

    _stack_start = _irq_stack_end;
    _stack_end = _stack_start + 0x1000; /* 64 KB */

    _irq_stack_size = _irq_stack_end - _irq_stack_start;
    _fiq_stack_size = _fiq_stack_end - _fiq_stack_start;
}
