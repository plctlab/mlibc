/* Some defines */
.equ MODE_FIQ, 0x11
.equ MODE_IRQ, 0x12
.equ MODE_SVC, 0x13

.section .vector_table, "x"
.global _Reset
.global _start
_Reset:
    ldr r1, =Reset_Handler
    msr ELR_hyp, r1
    mrs r1, cpsr_all
    and r1, r1, #0xFFFFFFE0    /* CPSR_MODE_MASK */
    orr r1, r1, #0x13          /* CPSR_MODE_SUPERVISOR */
    msr SPSR_hyp, r1
    eret

Reset_Handler:
    /* FIQ stack */
    msr cpsr_c, MODE_FIQ
    ldr r1, =_fiq_stack_start
    ldr sp, =_fiq_stack_end
    movw r0, #0xFEFE
    movt r0, #0xFEFE

fiq_loop:
    cmp r1, sp
    strlt r0, [r1], #4
    blt fiq_loop

    /* IRQ stack */
    msr cpsr_c, MODE_IRQ
    ldr r1, =_irq_stack_start
    ldr sp, =_irq_stack_end

irq_loop:
    cmp r1, sp
    strlt r0, [r1], #4
    blt irq_loop

    /* Supervisor mode */
    msr cpsr_c, MODE_SVC
    ldr r1, =_stack_start
    ldr sp, =_stack_end

stack_loop:
    cmp r1, sp
    strlt r0, [r1], #4
    blt stack_loop

    ldr r0, =__text_end
    ldr r1, =__data_start
    ldr r2, =__data_end

data_loop:
    cmp r1, r2
    ldrlt r3, [r0], #4
    strlt r3, [r1], #4
    blt data_loop
    
    mov r0, #0
    ldr r1, =__bss_start
    ldr r2, =__bss_end

bss_loop:
    cmp r1, r2
    strlt r0, [r1], #4
    blt bss_loop

    bl main
    b Abort_Exception

Abort_Exception:
    swi 0xFF



    