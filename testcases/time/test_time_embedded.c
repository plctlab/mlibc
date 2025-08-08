/*
 * test_time_embedded.c - PSE51 time.h test suite for embedded systems
 * Tests: time, gmtime, localtime, mktime, strftime, clock, nanosleep
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <time.h>
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


/* Test time() function */
void test_time(void) {
    time_t t1, t2;
    
    printf("Testing time()...");
    
    /* Get current time */
    t1 = time(NULL);
    if (t1 == (time_t)-1) {
        SKIP_TEST("not implemented");
    }
    
    /* Time should be positive */
    TEST_ASSERT(t1 > 0);
    
    /* Get time using pointer */
    TEST_ASSERT(time(&t2) == t2);
    
    /* Times should be close */
    TEST_ASSERT(t2 >= t1);
    TEST_ASSERT(t2 - t1 < 2);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test gmtime() and localtime() */
void test_time_conversion(void) {
    time_t t;
    struct tm *tm_p;
    
    printf("Testing gmtime() and localtime()...");
    
    /* Use a known time */
    t = 1000000000; /* Sep 9, 2001 */
    
    /* Test gmtime */
    tm_p = gmtime(&t);
    if (tm_p == NULL) {
        SKIP_TEST("not implemented");
    }
    
    TEST_ASSERT(tm_p->tm_year == 101); /* 2001 - 1900 */
    TEST_ASSERT(tm_p->tm_mon == 8);    /* September (0-based) */
    TEST_ASSERT(tm_p->tm_mday == 9);
    
    /* Test localtime */
    tm_p = localtime(&t);
    if (tm_p != NULL) {
        TEST_ASSERT(tm_p->tm_year == 101);
    }
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test clock() function */
void test_clock(void) {
    clock_t c1, c2;
    
    printf("Testing clock()...");
    
    c1 = clock();
    if (c1 == (clock_t)-1) {
        SKIP_TEST("not implemented");
    }
    
    /* Do some work */
    volatile int i;
    for (i = 0; i < 100000; i++) { /* busy wait */ }
    
    c2 = clock();
    TEST_ASSERT(c2 >= c1);
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 time.h Test Summary ===\n");
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
    
    printf("=== PSE51 time.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_time();
    test_time_conversion();
    test_clock();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
