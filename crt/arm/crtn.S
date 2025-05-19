.syntax unified

#if defined(__thumb__)
.section .init
    add sp, #8
    bx lr

.section .fini  
    add sp, #8
    bx lr
#else
.section .init
    pop {r0, lr}
    bx lr

.section .fini
    pop {r0, lr}
    bx lr
#endif
