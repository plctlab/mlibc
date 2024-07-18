/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020/12/12     bernard      The first version
 */
OUTPUT_ARCH( "riscv" )

MEMORY
{
   RAM : ORIGIN = 0x80200000, LENGTH = 0x1000000
}

ENTRY(Reset_Handler)
SECTIONS
{
    . = 0x80200000 ;

    __text_start = .;
    .text :
    {
        *(.text)
        *(.rodata*)
        *(.ARM.exidx)
        . = ALIGN(16);
    } > RAM
    __text_end = .;

    .data :
    {
        __data_start = .;
        *(.data)
        . = ALIGN(16);
        __data_end = .;
    } > RAM

    __stack_start = .;
    __stack_end = __stack_start + 0x1000;

    .bss :
    {
        __bss_start = .;
        *(.bss)
        . = ALIGN(16);
        __bss_end = .;
    } > RAM

}
