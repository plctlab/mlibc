/*
 * test_assert.c - PSE51 assert.h assertion test suite
 * Tests: assert macro
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for assertions
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../test_colors.h"

/* Jump buffer for testing assert failures */
static jmp_buf assert_jump;
static volatile int assert_failed = 0;

/* Signal handler for SIGABRT */
void sigabrt_handler(int sig) {
    (void)sig;
    assert_failed = 1;
    longjmp(assert_jump, 1);
}

/* Test basic assert functionality */
void test_assert_basic(void) {
    TEST_START("basic assert()");
    
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
    
    TEST_PASSED("Basic assert");
}

/* Test assert failure in child process */
void test_assert_failure(void) {
    pid_t pid;
    int status;
    
    TEST_START("assert() failure");
    
    pid = fork();
    if (pid == 0) {
        /* Child process - this should abort */
        assert(0);
        /* Should never reach here */
        _exit(0);
    }
    
    /* Parent waits for child */
    waitpid(pid, &status, 0);
    
    /* Child should have been terminated by SIGABRT */
    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
    
    /* Test with false expression */
    pid = fork();
    if (pid == 0) {
        /* Child process */
        int x = 5;
        assert(x == 10); /* False assertion */
        _exit(0);
    }
    
    waitpid(pid, &status, 0);
    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
    
    TEST_PASSED("Assert failure");
}

/* Test assert with NDEBUG */
void test_assert_ndebug(void) {
    TEST_START("assert() with NDEBUG");
    
    /* This test needs to be compiled with -DNDEBUG to work properly */
    #ifdef NDEBUG
    printf("  NDEBUG is defined - asserts should be disabled\n");
    
    /* These should not abort when NDEBUG is defined */
    assert(0);
    assert(1 == 2);
    
    /* If we get here, asserts were properly disabled */
    TEST_PASSED("NDEBUG - assertions disabled");
    #else
    printf("  NDEBUG is not defined - asserts are enabled\n");
    printf("  To test NDEBUG, recompile with -DNDEBUG\n");
    #endif
}

/* Test assert message output */
void test_assert_message(void) {
    pid_t pid;
    int pipefd[2];
    char buffer[1024];
    
    TEST_START("assert() message output");
    
    /* Create pipe to capture stderr */
    if (pipe(pipefd) == -1) {
        printf("  Cannot create pipe for message test\n");
        return;
    }
    
    pid = fork();
    if (pid == 0) {
        /* Child process */
        /* Redirect stderr to pipe */
        close(pipefd[0]);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        
        /* This should print assertion message to stderr */
        assert(2 + 2 == 5);
        _exit(0);
    }
    
    /* Parent process */
    close(pipefd[1]);
    
    /* Read assertion message */
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("  Assertion message: %s", buffer);
        
        /* Message should contain file name and line number */
        assert(strstr(buffer, "test_assert.c") != NULL || 
               strstr(buffer, __FILE__) != NULL);
        assert(strstr(buffer, "assert") != NULL);
    }
    
    close(pipefd[0]);
    waitpid(pid, NULL, 0);
    
    TEST_PASSED("Assert message");
}

/* Test assert with side effects */
void test_assert_side_effects(void) {
    TEST_START("assert() with side effects");
    
    int counter = 0;
    
    /* Assert should evaluate expression */
    assert(++counter == 1);
    assert(counter == 1);
    
    /* Multiple evaluations */
    assert((counter += 5) == 6);
    assert(counter == 6);
    
    /* With function calls */
    assert(printf("") >= 0); /* printf returns number of chars printed */
    
    #ifdef NDEBUG
    /* When NDEBUG is defined, expressions should NOT be evaluated */
    counter = 10;
    assert(++counter == 0); /* This should not increment counter */
    assert(counter == 10); /* Counter should be unchanged */
    #endif
    
    TEST_PASSED("Side effects");
}

/* Test assert in different contexts */
void test_assert_contexts(void) {
    TEST_START("assert() in different contexts");
    
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
        assert(0); /* Should not reach here */
    }
    
    /* With logical operators */
    assert(1 || 0);
    assert(!(0 && 0));
    assert(!0);
    
    /* With pointer checks */
    char *ptr = "test";
    assert(ptr != NULL);
    assert(*ptr == 't');
    
    TEST_PASSED("Context");
}

/* Test assert with complex expressions */
void test_assert_complex(void) {
    TEST_START("assert() with complex expressions");
    
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
    
    TEST_PASSED("Complex expression");
}

/* Helper function for demonstrating assert usage */
int divide(int a, int b) {
    assert(b != 0); /* Precondition */
    return a / b;
}

void test_assert_in_functions(void) {
    TEST_START("assert() in functions");
    
    /* Valid division */
    int result = divide(10, 2);
    assert(result == 5);
    
    /* Test in child process for division by zero */
    pid_t pid = fork();
    if (pid == 0) {
        /* This should trigger assert */
        divide(10, 0);
        _exit(0);
    }
    
    int status;
    waitpid(pid, &status, 0);
    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
    
    TEST_PASSED("Function assert");
}

/* Test common assert patterns */
void test_assert_patterns(void) {
    TEST_START("common assert() patterns");
    
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
    
    /* Pattern 4: Return value checking */
    FILE *fp = fopen("/dev/null", "w");
    assert(fp != NULL);
    fclose(fp);
    
    /* Pattern 5: Invariant checking */
    int count = 0;
    int max = 10;
    while (count < max) {
        assert(count >= 0 && count < max);
        count++;
    }
    assert(count == max);
    
    /* Pattern 6: Debug-only validation */
    int debug_counter = 0;
    assert(++debug_counter == 1); /* Only increments in debug builds */
    
    TEST_PASSED("Assert pattern");
}

/* Test assert with signal handling */
void test_assert_with_signals(void) {
    struct sigaction sa, old_sa;
    
    TEST_START("assert() with custom signal handling");
    
    /* Set up custom SIGABRT handler */
    sa.sa_handler = sigabrt_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGABRT, &sa, &old_sa) == -1) {
        printf("  Cannot set signal handler\n");
        return;
    }
    
    /* Set jump point */
    if (setjmp(assert_jump) == 0) {
        assert_failed = 0;
        /* This should trigger our handler */
        assert(0);
        /* Should not reach here */
        printf("  ERROR: Assert did not trigger!\n");
    } else {
        /* Jumped here from signal handler */
        assert(assert_failed == 1);
        printf("  Custom SIGABRT handler worked!\n");
    }
    
    /* Restore original handler */
    sigaction(SIGABRT, &old_sa, NULL);
    
    TEST_PASSED("Signal handling");
}

int main(void) {
    printf("=== PSE51 assert.h Test Suite ===\n\n");
    
    /* Run tests */
    test_assert_basic();
    test_assert_failure();
    test_assert_ndebug();
    test_assert_message();
    test_assert_side_effects();
    test_assert_contexts();
    test_assert_complex();
    test_assert_in_functions();
    test_assert_patterns();
    test_assert_with_signals();
    
    TEST_SUITE_PASSED("assert.h");
    return 0;
}
