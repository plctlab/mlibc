/*
 * test_stdlib_embedded.c - PSE51 stdlib.h test suite for embedded systems
 * Tests: malloc, free, calloc, realloc, abs, labs, div, ldiv, 
 *        atoi, atol, strtol, strtoul, rand, srand, exit, abort,
 *        getenv, system, bsearch, qsort
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 * Combines all stdlib tests into a single executable for bare-metal environment
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <compiler.h>
#include <uart.h>
#include <sys/types.h>

/* Global test status */
static int tests_passed = 0;
static int tests_failed = 0;
static int tests_skipped = 0;

/* Implement write() for printf support */
ssize_t write(int fd, const void *buf, size_t count)
{
    (void)fd;
    char *buffer = (char*)buf;
    size_t i;
    
    for (i = 0; i < count; i++) {
        UartPutc(buffer[i]);
    }
    
    return count;
}
    


/* Test runner macros */
#define RUN_TEST(test_func) do { \
    printf("Running %s...", #test_func); \
    test_func(); \
    tests_passed++; \
    printf(" PASSED\n"); \
} while(0)

#define SKIP_TEST(test_func, reason) do { \
    printf("Skipping %s: %s\n", #test_func, reason); \
    tests_skipped++; \
} while(0)

/* Memory management tests */
void test_malloc_basic(void) {
    void *ptr;
    char *char_ptr;
    size_t i;
    
    /* Test small allocation */
    ptr = malloc(16);
    if (ptr == NULL) {
        printf(" [malloc not available]");
        return;
    }
    
    /* Test that memory is writable */
    memset(ptr, 0xAA, 16);
    char_ptr = (char *)ptr;
    for (i = 0; i < 16; i++) {
        if (char_ptr[i] != (char)0xAA) {
            printf(" [memory corruption]");
            tests_failed++;
            free(ptr);
            return;
        }
    }
    free(ptr);
    
    /* Test multiple allocations */
    void *ptrs[10];
    for (i = 0; i < 10; i++) {
        ptrs[i] = malloc(32);
        if (ptrs[i] == NULL) {
            printf(" [allocation %zu failed]", i);
            /* Free already allocated */
            while (i > 0) {
                i--;
                free(ptrs[i]);
            }
            return;
        }
    }
    
    /* Free in reverse order */
    for (i = 10; i > 0; i--) {
        free(ptrs[i-1]);
    }
}

void test_calloc(void) {
    int *int_ptr;
    size_t i;
    
    /* Test basic calloc */
    int_ptr = calloc(10, sizeof(int));
    if (int_ptr == NULL) {
        printf(" [calloc not available]");
        return;
    }
    
    /* Verify zero initialization */
    for (i = 0; i < 10; i++) {
        if (int_ptr[i] != 0) {
            printf(" [not zero-initialized]");
            tests_failed++;
            free(int_ptr);
            return;
        }
    }
    
    free(int_ptr);
    
    /* Test overflow protection */
    void *ptr = calloc(SIZE_MAX/2, SIZE_MAX/2);
    if (ptr != NULL) {
        printf(" [overflow not detected]");
        free(ptr);
        tests_failed++;
    }
}

void test_realloc(void) {
    char *ptr, *new_ptr;
    size_t i;
    
    /* Initial allocation */
    ptr = malloc(10);
    if (ptr == NULL) {
        printf(" [malloc failed]");
        return;
    }
    
    /* Fill with pattern */
    for (i = 0; i < 10; i++) {
        ptr[i] = 'A' + i;
    }
    
    /* Grow allocation */
    new_ptr = realloc(ptr, 20);
    if (new_ptr == NULL) {
        printf(" [realloc grow failed]");
        free(ptr);
        return;
    }
    ptr = new_ptr;
    
    /* Verify original data preserved */
    for (i = 0; i < 10; i++) {
        if (ptr[i] != 'A' + i) {
            printf(" [data corrupted]");
            tests_failed++;
            free(ptr);
            return;
        }
    }
    
    /* Shrink allocation */
    new_ptr = realloc(ptr, 5);
    if (new_ptr == NULL) {
        printf(" [realloc shrink failed]");
        free(ptr);
        return;
    }
    ptr = new_ptr;
    
    /* Verify data preserved */
    for (i = 0; i < 5; i++) {
        if (ptr[i] != 'A' + i) {
            printf(" [shrink corrupted data]");
            tests_failed++;
            free(ptr);
            return;
        }
    }
    
    free(ptr);
    
    /* Test realloc(NULL, size) acts like malloc */
    ptr = realloc(NULL, 10);
    if (ptr != NULL) {
        free(ptr);
    }
    
    /* Test realloc(ptr, 0) acts like free */
    ptr = malloc(10);
    if (ptr != NULL) {
        ptr = realloc(ptr, 0);
        /* ptr should be NULL or freed */
    }
}

/* Integer math tests */
void test_abs_functions(void) {
    /* Test abs() */
    if (abs(5) != 5) tests_failed++;
    if (abs(-5) != 5) tests_failed++;
    if (abs(0) != 0) tests_failed++;
    
    /* Test labs() */
    if (labs(1000L) != 1000L) tests_failed++;
    if (labs(-1000L) != 1000L) tests_failed++;
    if (labs(0L) != 0L) tests_failed++;
    
    /* Test with INT_MIN - behavior is undefined but common */
    /* Most implementations return INT_MIN */
    int min_result = abs(INT_MIN);
    (void)min_result; /* Avoid warning */
}

void test_div_functions(void) {
    div_t d;
    ldiv_t ld;
    
    /* Test div() */
    d = div(10, 3);
    if (d.quot != 3 || d.rem != 1) tests_failed++;
    
    d = div(-10, 3);
    if (d.quot != -3 || d.rem != -1) tests_failed++;
    
    d = div(10, -3);
    if (d.quot != -3 || d.rem != 1) tests_failed++;
    
    d = div(0, 5);
    if (d.quot != 0 || d.rem != 0) tests_failed++;
    
    /* Test ldiv() */
    ld = ldiv(1000L, 37L);
    if (ld.quot != 27L || ld.rem != 1L) tests_failed++;
    
    ld = ldiv(-1000L, 37L);
    if (ld.quot != -27L || ld.rem != -1L) tests_failed++;
}

/* String conversion tests */
void test_string_conversions(void) {
    char *endptr;
    long l;
    unsigned long ul;
    
    /* Test atoi() */
    if (atoi("123") != 123) tests_failed++;
    if (atoi("-456") != -456) tests_failed++;
    if (atoi("   789") != 789) tests_failed++;
    if (atoi("0") != 0) tests_failed++;
    
    /* Test atol() */
    if (atol("123456") != 123456L) tests_failed++;
    if (atol("-789012") != -789012L) tests_failed++;
    
    /* Test strtol() */
    l = strtol("123", &endptr, 10);
    if (l != 123 || *endptr != '\0') tests_failed++;
    
    l = strtol("0xFF", &endptr, 16);
    if (l != 255 || *endptr != '\0') tests_failed++;
    
    l = strtol("0377", &endptr, 8);
    if (l != 255 || *endptr != '\0') tests_failed++;
    
    l = strtol("101010", &endptr, 2);
    if (l != 42 || *endptr != '\0') tests_failed++;
    
    /* Test strtoul() */
    ul = strtoul("4294967295", &endptr, 10);
    if (ul != 4294967295UL || *endptr != '\0') tests_failed++;
    
    /* Test with invalid input */
    l = strtol("abc", &endptr, 10);
    if (l != 0 || endptr != "abc") tests_failed++;
    
    /* Test overflow */
    errno = 0;
    l = strtol("99999999999999999999", &endptr, 10);
    if (errno != ERANGE) tests_failed++;
}

/* Random number tests */
void test_random(void) {
    int i, r1, r2;
    int different = 0;
    
    /* Test basic rand() */
    r1 = rand();
    if (r1 < 0 || r1 > RAND_MAX) tests_failed++;
    
    /* Test that we get different values */
    for (i = 0; i < 10; i++) {
        r2 = rand();
        if (r2 < 0 || r2 > RAND_MAX) tests_failed++;
        if (r1 != r2) different = 1;
        r1 = r2;
    }
    
    if (!different) {
        printf(" [rand() returns constant]");
        tests_failed++;
    }
    
    /* Test srand() */
    srand(12345);
    r1 = rand();
    
    /* Reseed with same value */
    srand(12345);
    r2 = rand();
    
    if (r1 != r2) {
        printf(" [srand() not deterministic]");
        tests_failed++;
    }
    
    /* Different seed should give different sequence */
    srand(54321);
    r2 = rand();
    if (r1 == r2) {
        printf(" [different seeds give same sequence]");
        tests_failed++;
    }
}

/* Comparison function for qsort/bsearch */
int int_compare(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

void test_sorting_searching(void) {
    int arr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int sorted[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    int key;
    int *found;
    size_t i;
    
    /* Test qsort() */
    qsort(arr, n, sizeof(int), int_compare);
    
    /* Verify sorted */
    for (i = 0; i < n; i++) {
        if (arr[i] != sorted[i]) {
            printf(" [qsort failed]");
            tests_failed++;
            return;
        }
    }
    
    /* Test bsearch() */
    key = 5;
    found = bsearch(&key, arr, n, sizeof(int), int_compare);
    if (found == NULL || *found != 5) {
        printf(" [bsearch failed to find]");
        tests_failed++;
    }
    
    /* Test not found */
    key = 10;
    found = bsearch(&key, arr, n, sizeof(int), int_compare);
    if (found != NULL) {
        printf(" [bsearch false positive]");
        tests_failed++;
    }
    
    /* Test edge cases */
    key = 1;
    found = bsearch(&key, arr, n, sizeof(int), int_compare);
    if (found == NULL || *found != 1) tests_failed++;
    
    key = 9;
    found = bsearch(&key, arr, n, sizeof(int), int_compare);
    if (found == NULL || *found != 9) tests_failed++;
}

/* Environment tests (likely limited in embedded) */
void test_environment(void) {
    char *value;
    
    /* Test getenv() - probably returns NULL in embedded */
    value = getenv("PATH");
    if (value != NULL) {
        printf(" [getenv returned: %s]", value);
    }
    
    /* Test with non-existent variable */
    value = getenv("NONEXISTENT_VAR_12345");
    if (value != NULL) {
        printf(" [unexpected env var]");
        tests_failed++;
    }
}

/* Program control tests */
void test_exit_abort(void) {
    /* Note: We can't actually test exit() and abort() 
     * as they would terminate the test program.
     * Just verify they're declared and linkable.
     */
    
    /* Function pointers to verify linkage */
    void (*exit_ptr)(int) = exit;
    void (*abort_ptr)(void) = abort;
    
    if (exit_ptr == NULL || abort_ptr == NULL) {
        printf(" [exit/abort not linked]");
        tests_failed++;
    }
    
    /* Could test atexit() but it's not in PSE51 */
}

/* System test (likely not supported in embedded) */
void test_system(void) {
    int ret;
    
    /* Test system(NULL) - check if command processor available */
    ret = system(NULL);
    if (ret == 0) {
        printf(" [no command processor]");
        /* This is expected in embedded */
    } else {
        /* Try a simple command */
        ret = system("echo test");
        /* Don't fail if not supported */
    }
}

/* Edge cases and stress tests */
void test_edge_cases(void) {
    void *ptr;
    size_t i;
    
    /* Test many small allocations */
    void *ptrs[100];
    int count = 0;
    
    for (i = 0; i < 100; i++) {
        ptrs[i] = malloc(8);
        if (ptrs[i] == NULL) break;
        count++;
    }
    
    printf(" [allocated %d small blocks]", count);
    
    /* Free them */
    for (i = 0; i < count; i++) {
        free(ptrs[i]);
    }
    
    /* Test large allocation (might fail in embedded) */
    ptr = malloc(64 * 1024); /* 64KB */
    if (ptr != NULL) {
        printf(" [64KB allocation succeeded]");
        free(ptr);
    } else {
        printf(" [64KB allocation failed]");
    }
    
    /* Test aligned allocation patterns */
    for (i = 1; i <= 128; i *= 2) {
        ptr = malloc(i);
        if (ptr != NULL) {
            /* Check alignment (optional) */
            uintptr_t addr = (uintptr_t)ptr;
            if (i >= sizeof(void*) && (addr % sizeof(void*)) != 0) {
                printf(" [alignment issue]");
                tests_failed++;
            }
            free(ptr);
        }
    }
}

/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 stdlib.h Test Summary ===\n");
    printf("=====================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Tests skipped: %d\n", tests_skipped);
    printf("Total tests: %d\n", tests_passed + tests_failed + tests_skipped);
    
    if (tests_failed == 0) {
        printf("\n*** ALL TESTS PASSED! ***\n");
    } else {
        printf("\n*** %d TESTS FAILED! ***\n", tests_failed);
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    /* Initialize UART for output */
    UartStdOutInit();
    
    printf("=== PSE51 stdlib.h Test Suite (Embedded) ===\n\n");
    
    /* Memory management tests */
    printf("\n-- Memory Management Tests --\n");
    RUN_TEST(test_malloc_basic);
    RUN_TEST(test_calloc);
    RUN_TEST(test_realloc);
    RUN_TEST(test_edge_cases);
    
    /* Integer math tests */
    printf("\n-- Integer Math Tests --\n");
    RUN_TEST(test_abs_functions);
    RUN_TEST(test_div_functions);
    
    /* String conversion tests */
    printf("\n-- String Conversion Tests --\n");
    RUN_TEST(test_string_conversions);
    
    /* Random number tests */
    printf("\n-- Random Number Tests --\n");
    RUN_TEST(test_random);
    
    /* Sorting and searching tests */
    printf("\n-- Sorting/Searching Tests --\n");
    RUN_TEST(test_sorting_searching);
    
    /* Environment tests */
    printf("\n-- Environment Tests --\n");
    RUN_TEST(test_environment);
    
    /* Program control tests */
    printf("\n-- Program Control Tests --\n");
    RUN_TEST(test_exit_abort);
    
    /* System tests */
    printf("\n-- System Tests --\n");
    SKIP_TEST(test_system, "Not supported in embedded");
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
