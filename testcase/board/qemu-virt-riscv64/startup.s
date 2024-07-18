.section .text
.global Reset_Handler
Reset_Handler:
    la sp, __stack_end
    call bss_init
    call main
    j .

bss_init:
    la t1, __bss_start
    la t2, __bss_end
    sub t2, t2, t1

    and t3, t2, 7
    li t4, -8
    and t2, t2, t4

clean_bss_loop_quad:
    beqz t2, clean_bss_loop_byte
    sd zero, 0(t1)
    addi t1, t1, 8
    addi t2, t2, -8
    j clean_bss_loop_quad

clean_bss_loop_byte:
    beqz t3, clean_bss_end
    sb zero, 0(t1)
    addi t1, t1, 1
    addi t3, t3, -1
    j clean_bss_loop_byte

clean_bss_end:
    ret

