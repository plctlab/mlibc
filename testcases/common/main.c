/*
 * main.c - Universal main entry point for all test cases
 * Supports both PC and embedded environments
 */

#include <stdio.h>
#include <stdlib.h>

/* Platform detection */
#ifdef __riscv
    #define EMBEDDED_TEST 1
#else
    #define EMBEDDED_TEST 0
#endif

/* Test function declaration - implemented in test_*.c files */
void run_tests(void);

#if EMBEDDED_TEST
/* External function for embedded initialization */
extern void initialise_monitor_handles(void);
#endif

int main(void) {
#if EMBEDDED_TEST
    initialise_monitor_handles();
    printf("\n=== Running Embedded Tests ===\n\n");
#endif

    run_tests();
    
    printf("\n=== Tests Completed ===\n");
    
    return 0;
}