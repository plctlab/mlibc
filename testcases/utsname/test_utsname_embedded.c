/*
 * test_utsname_embedded.c - PSE51 sys/utsname.h test suite for embedded systems
 * Tests: uname
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <sys/utsname.h>
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


/* Test uname() function */
void test_uname(void) {
    struct utsname uts;
    int ret;
    
    printf("Testing uname()...");
    
    ret = uname(&uts);
    if (ret < 0) {
        if (errno == ENOSYS) {
            SKIP_TEST("not implemented");
        }
        TEST_ASSERT(0);
        return;
    }
    
    /* All fields should be non-empty strings */
    TEST_ASSERT(strlen(uts.sysname) > 0);
    TEST_ASSERT(strlen(uts.release) > 0);
    TEST_ASSERT(strlen(uts.version) > 0);
    TEST_ASSERT(strlen(uts.machine) > 0);
    /* nodename can be empty */
    
    /* Print system info */
    printf("\n  System: %s %s %s", uts.sysname, uts.release, uts.machine);
    
    printf("\n  uname() PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 sys/utsname.h Test Summary ===\n");
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
    
    printf("=== PSE51 sys/utsname.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_uname();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
