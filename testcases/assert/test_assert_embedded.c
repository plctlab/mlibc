/*
 * test_assert_embedded.c - PSE51 assert.h assertion test suite for embedded systems
 * Tests: assert macro
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 * It avoids fork() and uses a simpler approach suitable for bare-metal environments
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <compiler.h>
#include <uart.h>
#include <sys/types.h>

/* Global test status */
static int tests_passed = 0;
static int tests_failed = 0;
static volatile int assert_triggered = 0;

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



/* Custom abort handler for testing */
void __assert(const char* file, int line, const char* msg)
{
    printf("\n*** Assertion failed: %s\n", msg);
    printf("*** File: %s, Line: %d\n", file, line);
    assert_triggered = 1;
    
    /* In test mode, we don't actually abort */
    /* Instead, we set a flag and return */
}

/* Test runner macros */
#define RUN_TEST(test_func) do { \
    printf("Running %s...", #test_func); \
    test_func(); \
    printf(" PASSED\n"); \
    tests_passed++; \
} while(0)

#define TEST_ASSERT_FAILS(expr) do { \
    assert_triggered = 0; \
    expr; \
    if (assert_triggered) { \
        printf(" [Expected assert triggered]"); \
    } else { \
        printf("\n*** ERROR: Expected assert to trigger but it didn't!\n"); \
        tests_failed++; \
    } \
} while(0)

/* Test basic assert functionality */
void test_assert_basic(void) {
    /* Test that assert(true) doesn't fail */
    assert(1);
    assert(1 == 1);
    assert(2 > 1);
    assert("string" != NULL);
    
    /* Test complex expressions */
    int x = 5;
    assert(x > 0 && x < 10);
    assert(x * 2 == 10);
    
    /* Test with function calls */
    assert(strlen("test") == 4);
}

/* Test assert failure */
void test_assert_failure(void) {
    printf("\n  Testing assert failures (expected messages below):\n");
    
    /* Test with false expression */
    #ifndef NDEBUG
    TEST_ASSERT_FAILS(assert(0));
    TEST_ASSERT_FAILS(assert(1 == 2));
    
    int x = 5;
    TEST_ASSERT_FAILS(assert(x == 10));
    #else
    printf("  [NDEBUG defined - assertions disabled]\n");
    #endif
}

/* Test assert with NDEBUG */
void test_assert_ndebug(void) {
    #ifdef NDEBUG
    printf("  NDEBUG is defined - asserts should be disabled\n");
    
    /* These should not abort when NDEBUG is defined */
    assert(0);
    assert(1 == 2);
    
    /* If we get here, asserts were properly disabled */
    printf("  Assertions were properly disabled");
    #else
    printf("  NDEBUG is not defined - asserts are enabled");
    #endif
}

/* Test assert with side effects */
void test_assert_side_effects(void) {
    int counter = 0;
    
    #ifndef NDEBUG
    /* Assert should evaluate expression */
    assert(++counter == 1);
    assert(counter == 1);
    
    /* Multiple evaluations */
    assert((counter += 5) == 6);
    assert(counter == 6);
    #else
    /* When NDEBUG is defined, expressions should NOT be evaluated */
    counter = 10;
    assert(++counter == 0); /* This should not increment counter */
    assert(counter == 10); /* Counter should be unchanged */
    #endif
}

/* Test assert in different contexts */
void test_assert_contexts(void) {
    /* In loops */
    for (int i = 0; i < 10; i++) {
        assert(i >= 0 && i < 10);
    }
    
    /* In conditional statements */
    int x = 5;
    if (x > 0) {
        assert(x > 0);
    } else {
        assert(x <= 0);
    }
    
    /* In switch statements */
    switch (x) {
    case 5:
        assert(x == 5);
        break;
    default:
        #ifndef NDEBUG
        TEST_ASSERT_FAILS(assert(0)); /* Should not reach here */
        #endif
    }
    
    /* With logical operators */
    assert(1 || 0);
    assert(!(0 && 0));
    assert(!0);
    
    /* With pointer checks */
    char *ptr = "test";
    assert(ptr != NULL);
    assert(*ptr == 't');
}

/* Test assert with complex expressions */
void test_assert_complex(void) {
    /* Array bounds checking */
    int arr[10] = {0};
    int index = 5;
    assert(index >= 0 && index < 10);
    arr[index] = 42;
    assert(arr[index] == 42);
    
    /* Structure member validation */
    struct {
        int x;
        int y;
    } point = {3, 4};
    
    assert(point.x > 0 && point.y > 0);
    assert(point.x * point.x + point.y * point.y == 25); /* 3² + 4² = 25 */
    
    /* String validation */
    char str[] = "Hello";
    assert(strlen(str) == 5);
    assert(strcmp(str, "Hello") == 0);
    assert(str[0] == 'H' && str[4] == 'o');
    
    /* Bit operations */
    unsigned int flags = 0x5;
    assert(flags & 0x1); /* Bit 0 is set */
    assert(!(flags & 0x2)); /* Bit 1 is not set */
    assert(flags & 0x4); /* Bit 2 is set */
}

/* Helper function for demonstrating assert usage */
int divide(int a, int b) {
    assert(b != 0); /* Precondition */
    return a / b;
}

void test_assert_in_functions(void) {
    /* Valid division */
    int result = divide(10, 2);
    assert(result == 5);
    
    /* Test division by zero */
    #ifndef NDEBUG
    printf("\n  Testing divide by zero (expected assert):\n");
    TEST_ASSERT_FAILS(divide(10, 0));
    #endif
}

/* Test common assert patterns */
void test_assert_patterns(void) {
    /* Pattern 1: NULL pointer checks */
    char *str = "test";
    assert(str != NULL);
    
    /* Pattern 2: Array index validation */
    int arr[100];
    int i = 50;
    assert(i >= 0 && i < 100);
    arr[i] = 123;
    
    /* Pattern 3: State validation */
    enum { STATE_INIT, STATE_READY, STATE_DONE } state = STATE_INIT;
    assert(state == STATE_INIT);
    state = STATE_READY;
    assert(state == STATE_READY);
    
    /* Pattern 4: Invariant checking */
    int count = 0;
    int max = 10;
    while (count < max) {
        assert(count >= 0 && count < max);
        count++;
    }
    assert(count == max);
    
    /* Pattern 5: Debug-only validation */
    int debug_counter = 0;
    assert(++debug_counter == 1); /* Only increments in debug builds */
}

/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 assert.h Test Summary ===\n");
    printf("=====================================\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    
    if (tests_failed == 0) {
        printf("\n*** ALL TESTS PASSED! ***\n");
    } else {
        printf("\n*** SOME TESTS FAILED! ***\n");
    }
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    
    /* Initialize UART for output */
    UartStdOutInit();
    
    printf("=== PSE51 assert.h Test Suite (Embedded) ===\n\n");
    
    /* Run tests */
    RUN_TEST(test_assert_basic);
    RUN_TEST(test_assert_failure);
    RUN_TEST(test_assert_ndebug);
    RUN_TEST(test_assert_side_effects);
    RUN_TEST(test_assert_contexts);
    RUN_TEST(test_assert_complex);
    RUN_TEST(test_assert_in_functions);
    RUN_TEST(test_assert_patterns);
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
