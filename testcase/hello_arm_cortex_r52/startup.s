.section .text
.global _start
.global main

_start:
    ldr sp, =_stack_top

    ldr r0, =_etext
    ldr r1, =_data
    ldr r2, =_edata
    
copy_data:
    cmp r1, r2
    ittt lt
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt copy_data

    ldr r0, =_bss
    ldr r1, =_ebss
    movs r2, #0

clear_bss:
    cmp r0, r1
    ittt lt
    strlt r2, [r0], #4
    blt clear_bss
    bl main

hang:
    b hang
