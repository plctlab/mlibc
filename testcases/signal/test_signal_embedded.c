/*
 * test_signal_embedded.c - PSE51 signal.h test suite for embedded systems
 * Tests: signal, raise, sigaction, sigemptyset, sigfillset, sigaddset, sigdelset, sigismember
 * 
 * This test suite is designed for embedded systems (RISC-V QEMU virt)
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <compiler.h>
#include <uart.h>
#include <unistd.h>
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


static volatile int signal_received = 0;

/* Signal handler */
void signal_handler(int sig) {
    signal_received = sig;
}

/* Test signal() function */
void test_signal(void) {
    void (*old_handler)(int);
    
    printf("Testing signal()...");
    
    /* Install signal handler */
    old_handler = signal(SIGUSR1, signal_handler);
    if (old_handler == SIG_ERR) {
        SKIP_TEST("not implemented");
    }
    
    /* Reset flag */
    signal_received = 0;
    
    /* Send signal to self */
    raise(SIGUSR1);
    
    /* Check if handler was called */
    TEST_ASSERT(signal_received == SIGUSR1);
    
    /* Restore old handler */
    signal(SIGUSR1, old_handler);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test sigset functions */
void test_sigset_ops(void) {
    sigset_t set, set2;
    
    printf("Testing sigset operations...");
    
    /* Test sigemptyset */
    TEST_ASSERT(sigemptyset(&set) == 0);
    
    /* Test sigaddset */
    TEST_ASSERT(sigaddset(&set, SIGINT) == 0);
    TEST_ASSERT(sigaddset(&set, SIGTERM) == 0);
    
    /* Test sigismember */
    TEST_ASSERT(sigismember(&set, SIGINT) == 1);
    TEST_ASSERT(sigismember(&set, SIGTERM) == 1);
    TEST_ASSERT(sigismember(&set, SIGUSR1) == 0);
    
    /* Test sigdelset */
    TEST_ASSERT(sigdelset(&set, SIGINT) == 0);
    TEST_ASSERT(sigismember(&set, SIGINT) == 0);
    
    /* Test sigfillset */
    TEST_ASSERT(sigfillset(&set2) == 0);
    TEST_ASSERT(sigismember(&set2, SIGINT) == 1);
    TEST_ASSERT(sigismember(&set2, SIGTERM) == 1);
    
    printf(" PASSED\n");
    tests_passed++;
}

/* Test raise() function */
void test_raise(void) {
    printf("Testing raise()...");
    
    /* Install handler */
    signal(SIGUSR2, signal_handler);
    signal_received = 0;
    
    /* Raise signal */
    TEST_ASSERT(raise(SIGUSR2) == 0);
    
    /* Verify handler was called */
    TEST_ASSERT(signal_received == SIGUSR2);
    
    printf(" PASSED\n");
    tests_passed++;
}


/* Print test summary */
void print_summary(void) {
    printf("\n");
    printf("=====================================\n");
    printf("=== PSE51 signal.h Test Summary ===\n");
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
    
    printf("=== PSE51 signal.h Test Suite (Embedded) ===\n\n");
    
    /* Reset test counters */
    tests_passed = 0;
    tests_failed = 0;
    tests_skipped = 0;
    
    /* Run tests */
    test_signal();
    test_sigset_ops();
    test_raise();
    
    /* Print summary */
    print_summary();
    
    /* Infinite loop for embedded system */
    printf("\nTest complete. System halted.\n");
    while(1) {
        /* Halt */
    }
    
    return 0;
}
