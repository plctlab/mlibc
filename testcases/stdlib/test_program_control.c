/*
 * test_program_control.c - PSE51 stdlib.h program control test suite
 * Tests: exit, _Exit, abort, atexit
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for program control functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include "../test_colors.h"
/* Global variables for atexit testing */
static int atexit_called = 0;
static int atexit_order[32];
static int atexit_count = 0;
static FILE *status_file = NULL;

/* atexit handler functions */
void atexit_handler1(void) {
    if (status_file) {
        fprintf(status_file, "Handler1\n");
        fflush(status_file);
    }
    atexit_order[atexit_count++] = 1;
}

void atexit_handler2(void) {
    if (status_file) {
        fprintf(status_file, "Handler2\n");
        fflush(status_file);
    }
    atexit_order[atexit_count++] = 2;
}

void atexit_handler3(void) {
    if (status_file) {
        fprintf(status_file, "Handler3\n");
        fflush(status_file);
    }
    atexit_order[atexit_count++] = 3;
}

/* Test exit() in child process */
void test_exit_child(void) {
    pid_t pid;
    int status;
    
    TEST_START("exit() in child process");
    
    /* Test normal exit with success */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        exit(EXIT_SUCCESS);
    }
    
    /* Parent process */
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == EXIT_SUCCESS);
    
    /* Test exit with custom code */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        exit(42);
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 42);
    
    /* Test exit with failure code */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        exit(EXIT_FAILURE);
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == EXIT_FAILURE);
    
    TEST_PASSED("exit()");
}

/* Test _Exit() in child process */
void test__Exit_child(void) {
    pid_t pid;
    int status;
    
    TEST_START("_Exit()");
    
    /* Test _Exit with success */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        /* _Exit should not call atexit handlers or flush buffers */
        FILE *fp = fopen("test__Exit.tmp", "w");
        if (fp) {
            fprintf(fp, "This should not be written\n");
            /* No fclose - buffer should not be flushed */
        }
        _Exit(0);
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    /* Check that buffer was not flushed */
    FILE *fp = fopen("test__Exit.tmp", "r");
    if (fp) {
        char buf[100];
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            /* Good - file is empty */
        }
        fclose(fp);
        remove("test__Exit.tmp");
    }
    
    /* Test _Exit with custom code */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        _Exit(123);
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 123);
    
    TEST_PASSED("_Exit()");
}

/* Test abort() in child process */
void test_abort_child(void) {
    pid_t pid;
    int status;
    
    TEST_START("abort()");
    
    /* Test basic abort */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        abort();
    }
    
    /* Parent process */
    waitpid(pid, &status, 0);
    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
    
    /* Test abort with signal handler */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        /* Even with handler, abort should still terminate */
        signal(SIGABRT, SIG_IGN);
        abort();
    }
    
    waitpid(pid, &status, 0);
    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
    
    TEST_PASSED("abort()");
}

/* Test atexit() in child process */
void test_atexit_child(void) {
    pid_t pid;
    int status;
    
    TEST_START("atexit()");
    
    /* Test basic atexit registration and calling */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        FILE *fp = fopen("test_atexit.tmp", "w");
        if (!fp) {
            _Exit(1);
        }
        
        status_file = fp;
        
        /* Register handlers - should be called in reverse order */
        int ret1 = atexit(atexit_handler1);
        int ret2 = atexit(atexit_handler2);
        int ret3 = atexit(atexit_handler3);
        
        if (ret1 != 0 || ret2 != 0 || ret3 != 0) {
            fprintf(fp, "atexit registration failed\n");
            fclose(fp);
            _Exit(1);
        }
        
        fprintf(fp, "Before exit\n");
        fflush(fp);
        
        /* This should trigger handlers in order: 3, 2, 1 */
        exit(0);
    }
    
    /* Parent process */
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    /* Check handler execution */
    FILE *fp = fopen("test_atexit.tmp", "r");
    if (fp) {
        char buf[100];
        int line = 0;
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            buf[strcspn(buf, "\n")] = 0;
            switch (line) {
                case 0:
                    assert(strcmp(buf, "Before exit") == 0);
                    break;
                case 1:
                    assert(strcmp(buf, "Handler3") == 0);
                    break;
                case 2:
                    assert(strcmp(buf, "Handler2") == 0);
                    break;
                case 3:
                    assert(strcmp(buf, "Handler1") == 0);
                    break;
            }
            line++;
        }
        assert(line == 4); /* All handlers called */
        fclose(fp);
        remove("test_atexit.tmp");
    }
    
    /* Test that _Exit doesn't call atexit handlers */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        FILE *fp = fopen("test_atexit2.tmp", "w");
        if (!fp) {
            _Exit(1);
        }
        
        status_file = fp;
        atexit(atexit_handler1);
        
        fprintf(fp, "Before _Exit\n");
        fflush(fp);
        
        _Exit(0); /* Should NOT call handler */
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    /* Check that handler was NOT called */
    fp = fopen("test_atexit2.tmp", "r");
    if (fp) {
        char buf[100];
        int lines = 0;
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            lines++;
        }
        assert(lines == 1); /* Only "Before _Exit" */
        fclose(fp);
        remove("test_atexit2.tmp");
    }
    
    TEST_PASSED("atexit()");
}

/* Test atexit() limit */
void test_atexit_limit(void) {
    pid_t pid;
    int status;
    
    TEST_START("atexit() limit");
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        int count = 0;
        int ret;
        
        /* Try to register many handlers */
        /* POSIX requires at least 32 */
        while (count < 100) {
            ret = atexit(atexit_handler1);
            if (ret != 0) {
                /* Hit the limit */
                break;
            }
            count++;
        }
        
        /* Should have registered at least 32 */
        if (count >= 32) {
            exit(0);
        } else {
            exit(1);
        }
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    printf("  atexit() limit test passed (>= 32 handlers)!\n");
}

/* Test exit code values */
void test_exit_codes(void) {
    pid_t pid;
    int status;
    int test_codes[] = {0, 1, 2, 127, 128, 255};
    size_t i;
    
    TEST_START("exit codes");
    
    for (i = 0; i < sizeof(test_codes)/sizeof(test_codes[0]); i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        }
        
        if (pid == 0) {
            /* Child process */
            exit(test_codes[i]);
        }
        
        /* Parent process */
        waitpid(pid, &status, 0);
        assert(WIFEXITED(status));
        /* Exit codes are masked to 8 bits */
        assert(WEXITSTATUS(status) == (test_codes[i] & 0xFF));
    }
    
    /* Test exit code wrapping */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        exit(256); /* Should wrap to 0 */
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        exit(257); /* Should wrap to 1 */
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 1);
    
    TEST_PASSED("Exit code");
}

/* Test interaction between functions */
void test_interaction(void) {
    pid_t pid;
    int status;
    
    TEST_START("function interactions");
    
    /* Test that abort doesn't call atexit handlers */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        FILE *fp = fopen("test_abort_atexit.tmp", "w");
        if (!fp) {
            _Exit(1);
        }
        
        status_file = fp;
        atexit(atexit_handler1);
        
        fprintf(fp, "Before abort\n");
        fflush(fp);
        
        abort(); /* Should NOT call handler */
    }
    
    waitpid(pid, &status, 0);
    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
    
    /* Check that handler was NOT called */
    FILE *fp = fopen("test_abort_atexit.tmp", "r");
    if (fp) {
        char buf[100];
        int lines = 0;
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            lines++;
        }
        assert(lines == 1); /* Only "Before abort" */
        fclose(fp);
        remove("test_abort_atexit.tmp");
    }
    
    TEST_PASSED("Interaction");
}

/* Test cleanup behavior */
void test_cleanup(void) {
    pid_t pid;
    int status;
    
    TEST_START("cleanup behavior");
    
    /* Test that exit() flushes buffers */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        FILE *fp = fopen("test_exit_flush.tmp", "w");
        if (!fp) {
            _Exit(1);
        }
        
        fprintf(fp, "This should be written");
        /* No fclose or fflush */
        
        exit(0); /* Should flush buffer */
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    /* Check that buffer was flushed */
    FILE *fp = fopen("test_exit_flush.tmp", "r");
    if (fp) {
        char buf[100];
        assert(fgets(buf, sizeof(buf), fp) != NULL);
        assert(strncmp(buf, "This should be written", 22) == 0);
        fclose(fp);
        remove("test_exit_flush.tmp");
    }
    
    /* Test that _Exit() doesn't flush buffers */
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        /* Child process */
        FILE *fp = fopen("test__Exit_noflush.tmp", "w");
        if (!fp) {
            _Exit(1);
        }
        
        fprintf(fp, "This should NOT be written");
        /* No fclose or fflush */
        
        _Exit(0); /* Should NOT flush buffer */
    }
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    /* Check that buffer was NOT flushed */
    fp = fopen("test__Exit_noflush.tmp", "r");
    if (fp) {
        /* File might exist but should be empty or very small */
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        assert(size < 10); /* Much less than the string length */
        fclose(fp);
        remove("test__Exit_noflush.tmp");
    }
    
    TEST_PASSED("Cleanup behavior");
}

/* Main test runner */
void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdlib.h Program Control Test Suite ===" COLOR_RESET "\n\n");
    
    /* Note: These tests use fork() to test program termination
     * without affecting the test runner itself */
    
    test_exit_child();
    test__Exit_child();
    test_abort_child();
    test_atexit_child();
    test_atexit_limit();
    test_exit_codes();
    test_interaction();
    test_cleanup();
    
    TEST_SUITE_PASSED("program control");
}
