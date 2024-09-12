/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/8/23  0Bitbiscuits the first version
 */
ENTRY(_start)

DEFAULT_FLASH_ADDR = 0x00000000;     /* Default flash addr is 0x00000000 */
DEFAULT_FLASH_SIZE = 0x00001000;     /* Default flash size is 4k */
DEFAULT_RAM_ADDR   = 0x20000000;     /* Default ram addr is 0x20000000 */
DEFAULT_RAM_SIZE   = 0x00010000;     /* Default ram size is 16k */
DEFAULT_ALIGNMENT  = 0x00000010;     /* Default ALIGNMENT */
DEFAULT_STACK_SIZE = 0x00000400;     /* Default stack size is 1k */
MEMORY
{
    flash (rx!w) :
        ORIGIN = DEFINED(__flash) ? __flash : DEFAULT_FLASH_ADDR,
        LENGTH = DEFINED(__flash_size) ? __flash_size : DEFAULT_FLASH_SIZE
    ram (w!rx) :
        ORIGIN = DEFINED(__ram) ? __ram : DEFAULT_RAM_ADDR,
        LENGTH = DEFINED(__ram_size) ? __ram_size : DEFAULT_RAM_SIZE
}

PHDRS
{
    text PT_LOAD;
    ram_init PT_LOAD;
    ram PT_LOAD;
}

SECTIONS
{
    PROVIDE(__stack = ORIGIN(ram) + LENGTH(ram));
    .text : {

        /* code */
        *(.text.unlikely .text.unlikely.*)
        *(.text.startup .text.startup.*)
        *(.text .text.* .opd .opd.*)
        *(.gnu.linkonce.t.*)
        KEEP (*(.init .init.*))
        KEEP (*(.fini .fini.*))
        __text_end = .;

        PROVIDE (__etext = __text_end);
        PROVIDE (_etext = __text_end);
        PROVIDE (etext = __text_end);
 
        /* Need to pre-align so that the symbols come after padding */
        . = ALIGN(DEFAULT_ALIGNMENT);

        PROVIDE_HIDDEN ( __init_array_start = . );
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)))
        KEEP (*(.init_array))
        PROVIDE_HIDDEN ( __init_array_end = . );

        PROVIDE_HIDDEN ( __fini_array_start = . );
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array .dtors))
        PROVIDE_HIDDEN ( __fini_array_end = . );

    } >flash AT>flash :text

    .rodata : {

        /* read-only data */
        *(.rdata)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.r.*)

        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)

    } >flash AT>flash :text

    /*
     * Data values which are preserved across reset
     */
    .preserve (NOLOAD) : {
        PROVIDE(__preserve_start__ = .);
        KEEP(*(SORT_BY_NAME(.preserve.*)))
        KEEP(*(.preserve))
        PROVIDE(__preserve_end__ = .);
    } >ram AT>ram :ram

    .data : ALIGN_WITH_INPUT {
        *(.data .data.*)    
        *(.gnu.linkonce.d.*)

        /* Need to pre-align so that the symbols come after padding */
        . = ALIGN(DEFAULT_ALIGNMENT);
        PROVIDE( __global_pointer$ = . + 0x800 );
        PROVIDE( _gp = . + 0x8000);
        *(.sdata .sdata.* .sdata2.*)
        *(.gnu.linkonce.s.*)
    } >ram AT>flash :ram_init
    PROVIDE(__data_start = ADDR(.data));
    PROVIDE(__data_source = LOADADDR(.data));
    PROVIDE(__data_end = .);
    PROVIDE(__data_source_end = LOADADDR(.data) + SIZEOF(.data));

    PROVIDE(__edata = __data_end);
    PROVIDE(_edata = __data_end);
    PROVIDE(edata = __data_end);
    PROVIDE(__data_size = __data_end - __data_start);
    PROVIDE(__data_source_size = __data_source_end - __data_source);

    .bss (NOLOAD) : {
        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)

        /* Align the heap */
        . = ALIGN(DEFAULT_ALIGNMENT);
        __bss_end = .;
    } >ram AT>ram :ram
    PROVIDE(__bss_start = ADDR(.bss));
    PROVIDE(__end = __bss_end);
    _end = __bss_end;
    PROVIDE(end = __bss_end);
    PROVIDE(__bss_size = __bss_end - __bss_start);

    /* Make the rest of memory available for heap storage */
    PROVIDE(__heap_start = __end);
    PROVIDE(__heap_end = __stack - (DEFINED(__stack_size) ? __stack_size : DEFAULT_STACK_SIZE));
    PROVIDE(__heap_size = __heap_end - __heap_start);

    /* Allow a minimum heap size to be specified */
    .heap (NOLOAD) : {
        . += (DEFINED(__heap_size_min) ? __heap_size_min : 0);
    } >ram :ram

    /* Define a stack region to make sure it fits in memory */
    .stack (NOLOAD) : {
        . += (DEFINED(__stack_size) ? __stack_size : DEFAULT_STACK_SIZE);
    } >ram :ram

    /* @C_START@ */
    /DISCARD/ : {
        *(.note .note.*)
        *(.eh_frame .eh_frame.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        *(.ARM.exidx*)
    }
    /* @C_END@ */
}