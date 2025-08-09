/*
 * embedded_startup.c - Common startup code for embedded tests
 */

#include <stdint.h>

/* Stack pointer value (defined in linker script) */
extern uint32_t _stack_top;

/* Start of BSS section */
extern uint32_t _bss_start;
extern uint32_t _bss_end;

/* Start of data section */
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _data_load;

/* Main function */
extern int main(void);

/* Reset handler */
void reset_handler(void) {
    uint32_t *src, *dst;
    
    /* Copy data section from flash to RAM */
    src = &_data_load;
    dst = &_data_start;
    while (dst < &_data_end) {
        *dst++ = *src++;
    }
    
    /* Clear BSS section */
    dst = &_bss_start;
    while (dst < &_bss_end) {
        *dst++ = 0;
    }
    
    /* Call main */
    main();
    
    /* Hang if main returns */
    while (1);
}

/* Vector table */
__attribute__((section(".vectors")))
const uint32_t vectors[] = {
    (uint32_t)&_stack_top,  /* Initial stack pointer */
    (uint32_t)reset_handler /* Reset handler */
};

/* Default interrupt handler */
void default_handler(void) {
    while (1);
}

/* Weak aliases for all interrupts */
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hardfault_handler(void) __attribute__((weak, alias("default_handler")));
void svc_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));