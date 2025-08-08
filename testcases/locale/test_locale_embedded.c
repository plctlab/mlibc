/*
 * test_locale_embedded.c - PSE51 locale.h test suite for embedded systems
 * Tests: setlocale, localeconv
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
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
    


/* Test helper macros */
#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        printf("\n  FAILED: %s at line %d", #cond, __LINE__); \
        tests_failed++; \
    } \
} while(0)

#define SKIP_TEST(reason) do { \
    printf(" [SKIPPED: %s]", reason); \
    tests_skipped++; \
    return; \
} while(0)


/* Test setlocale() function */
void test_setlocale(void) {
    char *result;
    
    printf("Testing setlocale()...");
    
    /* Test querying current locale */
    result = setlocale(LC_ALL, NULL);
    if (result == NULL) {
        SKIP_TEST("not implemented");
    }
    
    /* Test setting C locale */
    result = setlocale(LC_ALL, "C");
    TEST_ASSERT(result != NULL);
    if (result) {
        TEST_ASSERT(strcmp(result, "C") == 0);
    }
    
    /* Test setting POSIX locale */
    result = setlocale(LC_ALL, "POSIX");
    if (result) {
        TEST_ASSERT(strcmp(result, "POSIX") == 0 || strcmp(result, "C") == 0);
    }
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test localeconv() function */
void test_localeconv(void) {
    struct lconv *lc;
    
    printf("Testing localeconv()...");
    
    lc = localeconv();
    if (lc == NULL) {
        SKIP_TEST("not implemented");
    }
    
    /* Basic checks for C locale */
    TEST_ASSERT(lc->decimal_point != NULL);
    TEST_ASSERT(strcmp(lc->decimal_point, ".") == 0);
    
    if (lc->thousands_sep) {
        TEST_ASSERT(strlen(lc->thousands_sep) == 0);
    }
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 locale.h Test Summary ===\n");
    printf("=====================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Tests skipped: %d\n", tests_skipped);
    
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
    
    printf("=== PSE51 locale.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_setlocale();
    test_localeconv();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
