/*
 * Copyright (c) mlibc & plct lab
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024/7/2    0Bitbiscuits the first version
 */
.equ MODE_SVC, 0x13

.section .vector_table, "x"
.global _Reset
.global _start
_Reset:
    b Reset_Handler

.section .text
Reset_Handler:
    msr     tpidr_el1, xzr

    mrs     x0, CurrentEL           /* CurrentEL Register. bit 2, 3. Others reserved */
    lsr     x0, x0, #2
    and     x0, x0, #3

    cmp     x0, #1
    bne     Abort_Exception

    bl      .init_cpu_sys
    bl      .stack_init
    bl      .data_init
    bl      .bss_init

    bl      main
    b       Abort_Exception

Abort_Exception:
    wfe
    b Abort_Exception

.init_cpu_sys:
    mrs     x0, sctlr_el1
    bic     x0, x0, #(3 << 3)       /* Disable SP Alignment check */
    bic     x0, x0, #(1 << 1)       /* Disable Alignment check */
    bic     x0, x0, #1              /* Disable MMU */
    msr     sctlr_el1, x0

    /* Avoid trap from SIMD or float point instruction */
    mov     x0, #0x00300000         /* Don't trap any SIMD/FP instructions in both EL0 and EL1 */
    msr     cpacr_el1, x0

    /* Applying context change */
    dsb     ish
    isb

    ret


.stack_init:
    msr     spsel, #1
    ldr     x0, =_stack_end
    mov     sp, x0

    ret

.data_init:
    ldr     x1, =__data_start
    ldr     x2, =__data_end
    ldr     x5, =__text_end

    sub     x2, x2, x1              /* get data size */
    and     x3, x2, #7              /* Get low 3 digits */
    ldr     x4, =~0x7               
    and     x2, x2, x4              /* keep high digits */

.data_loop_quad:
    cbz     x2, .data_loop_byte
    ldr     x6, [x5], #8            /* load data to x6 */
    str     x6, [x1], #8            /* store data to [x1] */
    sub     x2, x2, #8
    b       .data_loop_quad

.data_loop_byte:
    cbz     x3, .data_loop_end
    ldrb    w6, [x5], #1
    strb    w6, [x1], #1
    sub     x3, x3, #1
    b       .data_loop_byte

.data_loop_end:
    ret


.bss_init:
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
    sub     x2, x2, x1              /* Get bss size */

    and     x3, x2, #7              /* x3 is < 7 */
    ldr     x4, =~0x7
    and     x2, x2, x4              /* Mask ~7 */

.clean_bss_loop_quad:
    cbz     x2, .clean_bss_loop_byte
    str     xzr, [x1], #8
    sub     x2, x2, #8
    b       .clean_bss_loop_quad

.clean_bss_loop_byte:
    cbz     x3, .clean_bss_end
    strb    wzr, [x1], #1
    sub     x3, x3, #1
    b       .clean_bss_loop_byte

.clean_bss_end:
    ret



