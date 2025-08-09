/*
 * test_signal.c - PSE51 signal.h signal handling test suite
 * Tests: kill, raise, sigaction, sigaddset, sigdelset, sigemptyset,
 *        sigfillset, sigismember, signal, sigpending, sigprocmask,
 *        sigsuspend
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for signal handling
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../test_colors.h"
/* Global variables for signal handling */
static volatile sig_atomic_t signal_count = 0;
static volatile sig_atomic_t last_signal = 0;
static volatile sig_atomic_t sigusr1_received = 0;
static volatile sig_atomic_t sigusr2_received = 0;
static volatile sig_atomic_t sigalrm_received = 0;
static volatile sig_atomic_t sigchld_received = 0;

/* Simple signal handler */
void simple_handler(int sig) {
    signal_count++;
    last_signal = sig;
}

/* Specific signal handlers */
void sigusr1_handler(int sig) {
    (void)sig;
    sigusr1_received = 1;
}

void sigusr2_handler(int sig) {
    (void)sig;
    sigusr2_received = 1;
}

void sigalrm_handler(int sig) {
    (void)sig;
    sigalrm_received = 1;
}

void sigchld_handler(int sig) {
    (void)sig;
    sigchld_received = 1;
}

/* Test sigset_t manipulation functions */
void test_sigset_functions(void) {
    sigset_t set1, set2;
    int ret;
    
    TEST_START("sigset_t manipulation functions");
    
    /* Test sigemptyset */
    ret = sigemptyset(&set1);
    assert(ret == 0);
    
    /* Verify set is empty */
    assert(sigismember(&set1, SIGINT) == 0);
    assert(sigismember(&set1, SIGTERM) == 0);
    assert(sigismember(&set1, SIGUSR1) == 0);
    
    /* Test sigfillset */
    ret = sigfillset(&set2);
    assert(ret == 0);
    
    /* Verify set is full */
    assert(sigismember(&set2, SIGINT) == 1);
    assert(sigismember(&set2, SIGTERM) == 1);
    assert(sigismember(&set2, SIGUSR1) == 1);
    
    /* Test sigaddset */
    ret = sigaddset(&set1, SIGINT);
    assert(ret == 0);
    ret = sigaddset(&set1, SIGUSR1);
    assert(ret == 0);
    
    /* Verify signals were added */
    assert(sigismember(&set1, SIGINT) == 1);
    assert(sigismember(&set1, SIGUSR1) == 1);
    assert(sigismember(&set1, SIGTERM) == 0);
    
    /* Test sigdelset */
    ret = sigdelset(&set2, SIGINT);
    assert(ret == 0);
    
    /* Verify signal was removed */
    assert(sigismember(&set2, SIGINT) == 0);
    assert(sigismember(&set2, SIGTERM) == 1);
    
    /* Test with invalid signal number */
    ret = sigaddset(&set1, 0);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    ret = sigaddset(&set1, 999);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Test sigismember with empty and full sets */
    sigemptyset(&set1);
    sigfillset(&set2);
    
    int sig;
    for (sig = 1; sig < 32; sig++) {
        assert(sigismember(&set1, sig) == 0);
        assert(sigismember(&set2, sig) == 1);
    }
    
    TEST_PASSED("sigset_t manipulation");
}

/* Test signal() function */
void test_signal(void) {
    void (*old_handler)(int);
    
    TEST_START("signal()");
    
    /* Reset counters */
    signal_count = 0;
    last_signal = 0;
    
    /* Install signal handler */
    old_handler = signal(SIGUSR1, simple_handler);
    assert(old_handler != SIG_ERR);
    
    /* Send signal to self */
    raise(SIGUSR1);
    
    /* Verify handler was called */
    assert(signal_count == 1);
    assert(last_signal == SIGUSR1);
    
    /* Test SIG_IGN */
    old_handler = signal(SIGUSR2, SIG_IGN);
    assert(old_handler != SIG_ERR);
    
    /* This signal should be ignored */
    raise(SIGUSR2);
    assert(signal_count == 1); /* Count unchanged */
    
    /* Test SIG_DFL */
    old_handler = signal(SIGUSR1, SIG_DFL);
    assert(old_handler == simple_handler);
    
    /* Test invalid signal */
    old_handler = signal(0, simple_handler);
    assert(old_handler == SIG_ERR);
    assert(errno == EINVAL);
    
    old_handler = signal(999, simple_handler);
    assert(old_handler == SIG_ERR);
    assert(errno == EINVAL);
    
    /* Test signals that cannot be caught */
    old_handler = signal(SIGKILL, simple_handler);
    assert(old_handler == SIG_ERR);
    assert(errno == EINVAL);
    
    old_handler = signal(SIGSTOP, simple_handler);
    assert(old_handler == SIG_ERR);
    assert(errno == EINVAL);
    
    /* Restore default handlers */
    signal(SIGUSR1, SIG_DFL);
    signal(SIGUSR2, SIG_DFL);
    
    TEST_PASSED("signal()");
}

/* Test sigaction() function */
void test_sigaction(void) {
    struct sigaction sa, old_sa;
    int ret;
    
    TEST_START("sigaction()");
    
    /* Reset counters */
    signal_count = 0;
    last_signal = 0;
    sigusr1_received = 0;
    
    /* Set up sigaction structure */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    /* Install handler */
    ret = sigaction(SIGUSR1, &sa, &old_sa);
    if (ret == -1 && errno == ENOSYS) {
        printf("  sigaction() not implemented (ENOSYS)\n");
        return;
    }
    assert(ret == 0);
    
    /* Send signal */
    raise(SIGUSR1);
    assert(sigusr1_received == 1);
    
    /* Test with flags - SA_RESTART may not be defined in minimal libc */
    sa.sa_handler = sigusr2_handler;
#ifdef SA_RESTART
    sa.sa_flags = SA_RESTART;
#else
    sa.sa_flags = 0;  /* No flags if SA_RESTART not available */
#endif
    ret = sigaction(SIGUSR2, &sa, NULL);
    assert(ret == 0);
    
    /* Test retrieving current action */
    ret = sigaction(SIGUSR1, NULL, &old_sa);
    assert(ret == 0);
    assert(old_sa.sa_handler == sigusr1_handler);
    
    /* Test with signal mask */
    sa.sa_handler = simple_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR2); /* Block SIGUSR2 during handler */
    sa.sa_flags = 0;
    
    ret = sigaction(SIGUSR1, &sa, NULL);
    assert(ret == 0);
    
    /* Test SIG_IGN */
    sa.sa_handler = SIG_IGN;
    ret = sigaction(SIGUSR2, &sa, NULL);
    assert(ret == 0);
    
    sigusr2_received = 0;
    raise(SIGUSR2);
    assert(sigusr2_received == 0); /* Should be ignored */
    
    /* Test invalid signal */
    ret = sigaction(0, &sa, NULL);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Test SIGKILL/SIGSTOP */
    ret = sigaction(SIGKILL, &sa, NULL);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Restore defaults */
    sa.sa_handler = SIG_DFL;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    TEST_PASSED("sigaction()");
}

/* Test raise() function */
void test_raise(void) {
    int ret;
    
    TEST_START("raise()");
    
    /* Set up handler */
    signal_count = 0;
    signal(SIGUSR1, simple_handler);
    
    /* Test raise */
    ret = raise(SIGUSR1);
    assert(ret == 0);
    assert(signal_count == 1);
    assert(last_signal == SIGUSR1);
    
    /* Test with ignored signal */
    signal(SIGUSR2, SIG_IGN);
    ret = raise(SIGUSR2);
    assert(ret == 0);
    assert(signal_count == 1); /* No change */
    
    /* Test invalid signal */
    ret = raise(0);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    ret = raise(999);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Restore defaults */
    signal(SIGUSR1, SIG_DFL);
    signal(SIGUSR2, SIG_DFL);
    
    TEST_PASSED("raise()");
}

/* Test kill() function */
void test_kill(void) {
    pid_t pid;
    int ret;
    int status;
    
    TEST_START("kill()");
    
    /* Test sending signal to self */
    signal_count = 0;
    signal(SIGUSR1, simple_handler);
    
    ret = kill(getpid(), SIGUSR1);
    assert(ret == 0);
    assert(signal_count == 1);
    
    /* Test with pid 0 (process group) */
    ret = kill(0, 0);
    assert(ret == 0); /* Signal 0 just checks if we can send */
    
    /* Test invalid pid */
    ret = kill(-999999, SIGUSR1);
    assert(ret == -1);
    assert(errno == ESRCH);
    
    /* Test invalid signal */
    ret = kill(getpid(), -1);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Test with child process */
    pid = fork();
    if (pid == 0) {
        /* Child: wait for signal */
        signal(SIGUSR1, sigusr1_handler);
        sigusr1_received = 0;
        
        pause(); /* Wait for signal */
        
        if (sigusr1_received) {
            _exit(0);
        } else {
            _exit(1);
        }
    }
    
    /* Parent: send signal to child */
    sleep(1); /* Give child time to set up */
    ret = kill(pid, SIGUSR1);
    assert(ret == 0);
    
    waitpid(pid, &status, 0);
    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == 0);
    
    /* Test permission denied (try to signal init) */
    if (getuid() != 0) { /* Not root */
        ret = kill(1, SIGUSR1);
        if (ret == -1) {
            assert(errno == EPERM);
        }
    }
    
    /* Restore default */
    signal(SIGUSR1, SIG_DFL);
    
    TEST_PASSED("kill()");
}

/* Test sigprocmask() function */
void test_sigprocmask(void) {
    sigset_t set, oldset;
    int ret;
    
    TEST_START("sigprocmask()");
    
    /* Set up handler */
    sigusr1_received = 0;
    signal(SIGUSR1, sigusr1_handler);
    
    /* Block SIGUSR1 */
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    
    ret = sigprocmask(SIG_BLOCK, &set, &oldset);
    if (ret == -1 && errno == ENOSYS) {
        printf("  sigprocmask() not implemented (ENOSYS)\n");
        signal(SIGUSR1, SIG_DFL);
        return;
    }
    assert(ret == 0);
    
    /* Signal should be blocked */
    raise(SIGUSR1);
    assert(sigusr1_received == 0);
    
    /* Unblock signal */
    ret = sigprocmask(SIG_UNBLOCK, &set, NULL);
    assert(ret == 0);
    
    /* Now signal should be delivered */
    assert(sigusr1_received == 1);
    
    /* Test SIG_SETMASK */
    sigusr1_received = 0;
    sigusr2_received = 0;
    signal(SIGUSR2, sigusr2_handler);
    
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    
    ret = sigprocmask(SIG_SETMASK, &set, NULL);
    assert(ret == 0);
    
    /* Both signals blocked */
    raise(SIGUSR1);
    raise(SIGUSR2);
    assert(sigusr1_received == 0);
    assert(sigusr2_received == 0);
    
    /* Unblock all */
    sigemptyset(&set);
    ret = sigprocmask(SIG_SETMASK, &set, NULL);
    assert(ret == 0);
    
    /* Signals delivered */
    assert(sigusr1_received == 1);
    assert(sigusr2_received == 1);
    
    /* Test retrieving current mask */
    ret = sigprocmask(SIG_BLOCK, NULL, &oldset);
    assert(ret == 0);
    
    /* Test invalid how parameter */
    ret = sigprocmask(999, &set, NULL);
    assert(ret == -1);
    assert(errno == EINVAL);
    
    /* Restore defaults */
    signal(SIGUSR1, SIG_DFL);
    signal(SIGUSR2, SIG_DFL);
    
    TEST_PASSED("sigprocmask()");
}

/* Test sigpending() function */
void test_sigpending(void) {
    sigset_t set, pending;
    int ret;
    
    TEST_START("sigpending()");
    
    /* Block SIGUSR1 */
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    
    ret = sigprocmask(SIG_BLOCK, &set, NULL);
    if (ret == -1 && errno == ENOSYS) {
        printf("  sigpending() test skipped (sigprocmask not available)\n");
        return;
    }
    
    /* Check no signals pending */
    ret = sigpending(&pending);
    if (ret == -1 && errno == ENOSYS) {
        printf("  sigpending() not implemented (ENOSYS)\n");
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        return;
    }
    assert(ret == 0);
    assert(sigismember(&pending, SIGUSR1) == 0);
    
    /* Send blocked signal */
    raise(SIGUSR1);
    
    /* Check signal is pending */
    ret = sigpending(&pending);
    assert(ret == 0);
    assert(sigismember(&pending, SIGUSR1) == 1);
    
    /* Unblock signal */
    signal(SIGUSR1, sigusr1_handler);
    sigusr1_received = 0;
    
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    assert(sigusr1_received == 1);
    
    /* Check no longer pending */
    ret = sigpending(&pending);
    assert(ret == 0);
    assert(sigismember(&pending, SIGUSR1) == 0);
    
    signal(SIGUSR1, SIG_DFL);
    
    TEST_PASSED("sigpending()");
}

/* Test sigsuspend() function */
void test_sigsuspend(void) {
    sigset_t mask, oldmask;
    int ret;
    
    TEST_START("sigsuspend()");
    
    /* Set up alarm handler */
    sigalrm_received = 0;
    signal(SIGALRM, sigalrm_handler);
    
    /* Block SIGALRM initially */
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    
    /* Set alarm */
    alarm(1);
    
    /* Wait with SIGALRM unblocked */
    sigemptyset(&mask);
    
    ret = sigsuspend(&mask);
    if (ret == -1 && errno == ENOSYS) {
        printf("  sigsuspend() not implemented (ENOSYS)\n");
        alarm(0);
        signal(SIGALRM, SIG_DFL);
        sigprocmask(SIG_SETMASK, &oldmask, NULL);
        return;
    }
    
    /* Should return -1 with EINTR */
    assert(ret == -1);
    assert(errno == EINTR);
    assert(sigalrm_received == 1);
    
    /* Restore mask and handler */
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    signal(SIGALRM, SIG_DFL);
    
    TEST_PASSED("sigsuspend()");
}

/* Test SIGCHLD handling */
void test_sigchld(void) {
    pid_t pid;
    int status;
    
    TEST_START("SIGCHLD handling");
    
    /* Set up SIGCHLD handler */
    sigchld_received = 0;
    signal(SIGCHLD, sigchld_handler);
    
    /* Fork child that exits immediately */
    pid = fork();
    if (pid == 0) {
        _exit(0);
    }
    
    /* Parent waits for SIGCHLD */
    sleep(1);
    assert(sigchld_received == 1);
    
    /* Reap child */
    waitpid(pid, &status, 0);
    
    /* Test with child that's stopped */
    /* Note: SIGSTOP/SIGCONT testing would be more complex */
    
    signal(SIGCHLD, SIG_DFL);
    
    TEST_PASSED("SIGCHLD");
}

/* Test signal delivery order and reliability */
void test_signal_reliability(void) {
    sigset_t set;
    
    TEST_START("signal reliability");
    
    /* Reset counters */
    signal_count = 0;
    
    /* Block all signals temporarily */
    sigfillset(&set);
    sigdelset(&set, SIGKILL);
    sigdelset(&set, SIGSTOP);
    sigprocmask(SIG_SETMASK, &set, NULL);
    
    /* Set up handler */
    signal(SIGUSR1, simple_handler);
    
    /* Send multiple signals while blocked */
    raise(SIGUSR1);
    raise(SIGUSR1);
    raise(SIGUSR1);
    
    /* Unblock signals */
    sigemptyset(&set);
    sigprocmask(SIG_SETMASK, &set, NULL);
    
    /* Standard signals are not queued, so we might get only one */
    assert(signal_count >= 1);
    
    signal(SIGUSR1, SIG_DFL);
    
    TEST_PASSED("Signal reliability");
}

/* Test signal handler with SA_SIGINFO */
void test_siginfo_handler(void) {
    struct sigaction sa;
    
    TEST_START("SA_SIGINFO handler");
    
    /* This is optional in PSE51, so just test if supported */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = simple_handler;
    sa.sa_flags = SA_SIGINFO; /* May not be supported */
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        printf("  SA_SIGINFO not supported\n");
    } else {
        printf("  SA_SIGINFO supported\n");
        
        /* Restore default */
        sa.sa_handler = SIG_DFL;
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
    }
}

/* Test common signal patterns */
void test_signal_patterns(void) {
    TEST_START("common signal patterns");
    
    /* Pattern 1: Ignoring signals during critical section */
    struct sigaction sa, old_sa;
    
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, &old_sa);
    /* Critical section - SIGINT ignored */
    sigaction(SIGINT, &old_sa, NULL);
    
    /* Pattern 2: Timeout using alarm */
    signal(SIGALRM, sigalrm_handler);
    sigalrm_received = 0;
    
    alarm(2);
    /* Do some work that might hang */
    sleep(1);
    alarm(0); /* Cancel alarm */
    
    signal(SIGALRM, SIG_DFL);
    
    /* Pattern 3: Graceful shutdown */
    signal(SIGTERM, simple_handler);
    /* In real app, would set a flag to exit main loop */
    signal(SIGTERM, SIG_DFL);
    
    TEST_PASSED("Signal pattern");
}




/* Run all tests */
void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 signal.h Test Suite ===" COLOR_RESET "\n\n");
    
    test_sigset_functions();
    test_signal();
    test_sigaction();
    test_raise();
    test_kill();
    test_sigprocmask();
    test_sigpending();
    test_sigsuspend();
    test_sigchld();
    test_signal_reliability();
    test_siginfo_handler();
    test_signal_patterns();
    
    TEST_SUITE_PASSED("signal.h");
}
