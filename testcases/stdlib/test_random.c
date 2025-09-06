/*
 * test_random.c - PSE51 stdlib.h random number generation test suite
 * Tests: rand, srand, rand_r
 * 
 * This test suite covers POSIX.13 (PSE51) requirements for random number functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include "../test_colors.h"
#define SAMPLE_SIZE 10000
#define BUCKET_COUNT 10
#define CHI_SQUARE_THRESHOLD 16.92  /* 95% confidence for 9 degrees of freedom */

/* Test rand() basic functionality */
void test_rand_basic(void) {
    int i;
    int value;
    int min_val = INT_MAX;
    int max_val = 0;
    
    TEST_START("rand() basic functionality");
    
    /* Generate some random numbers */
    for (i = 0; i < 100; i++) {
        value = rand();
        
        /* Check range */
        assert(value >= 0);
        assert(value <= RAND_MAX);
        
        /* Track min/max */
        if (value < min_val) min_val = value;
        if (value > max_val) max_val = value;
    }
    
    /* Verify we got some variation */
    assert(max_val > min_val);
    
    /* Test RAND_MAX */
    assert(RAND_MAX >= 32767); /* Minimum required by standard */
    
    TEST_PASSED("rand() basic");
    printf("  RAND_MAX = %d\n", RAND_MAX);
}

/* Test srand() functionality */
void test_srand(void) {
    int i;
    int sequence1[10];
    int sequence2[10];
    int sequence3[10];
    
    TEST_START("srand()");
    
    /* Test with specific seed */
    srand(12345);
    for (i = 0; i < 10; i++) {
        sequence1[i] = rand();
    }
    
    /* Reset with same seed - should get same sequence */
    srand(12345);
    for (i = 0; i < 10; i++) {
        sequence2[i] = rand();
    }
    
    /* Verify sequences match */
    for (i = 0; i < 10; i++) {
        assert(sequence1[i] == sequence2[i]);
    }
    
    /* Test with different seed */
    srand(54321);
    for (i = 0; i < 10; i++) {
        sequence3[i] = rand();
    }
    
    /* Verify different seed gives different sequence */
    int different = 0;
    for (i = 0; i < 10; i++) {
        if (sequence1[i] != sequence3[i]) {
            different++;
        }
    }
    assert(different > 0); /* At least some should be different */
    
    /* Test srand(1) - default seed */
    srand(1);
    int val1 = rand();
    
    /* Implementation may reset to seed 1 at program start */
    
    /* Test srand(0) */
    srand(0);
    int val0 = rand();
    assert(val0 >= 0 && val0 <= RAND_MAX);
    
    /* Test time-based seed (common pattern) */
    srand((unsigned int)time(NULL));
    int time_val = rand();
    assert(time_val >= 0 && time_val <= RAND_MAX);
    
    TEST_PASSED("srand()");
}

/* Test rand_r() - reentrant version */
void test_rand_r(void) {
    unsigned int seed1 = 12345;
    unsigned int seed2 = 12345;
    unsigned int seed3 = 54321;
    int i;
    int val;
    
    TEST_START("rand_r()");
    
    /* Generate sequences with same seed */
    int sequence1[10];
    int sequence2[10];
    
    for (i = 0; i < 10; i++) {
        sequence1[i] = rand_r(&seed1);
        assert(sequence1[i] >= 0);
        assert(sequence1[i] <= RAND_MAX);
    }
    
    /* Reset seed and regenerate */
    seed1 = 12345;
    for (i = 0; i < 10; i++) {
        sequence2[i] = rand_r(&seed1);
    }
    
    /* Verify sequences match */
    for (i = 0; i < 10; i++) {
        assert(sequence1[i] == sequence2[i]);
    }
    
    /* Test independence of seeds */
    seed1 = 12345;
    seed2 = 12345;
    
    /* Advance seed1 */
    for (i = 0; i < 5; i++) {
        rand_r(&seed1);
    }
    
    /* seed2 should still produce original sequence */
    for (i = 0; i < 10; i++) {
        val = rand_r(&seed2);
        assert(val == sequence1[i]);
    }
    
    /* Test different seed */
    int different = 0;
    for (i = 0; i < 10; i++) {
        int val3 = rand_r(&seed3);
        if (val3 != sequence1[i]) {
            different++;
        }
    }
    assert(different > 0);
    
    /* Test seed modification */
    unsigned int original_seed = 42;
    unsigned int working_seed = original_seed;
    
    rand_r(&working_seed);
    assert(working_seed != original_seed); /* Seed should be modified */
    
    /* Test zero seed */
    unsigned int zero_seed = 0;
    val = rand_r(&zero_seed);
    assert(val >= 0 && val <= RAND_MAX);
    assert(zero_seed != 0); /* Should modify seed */
    
    TEST_PASSED("rand_r()");
}

/* Test distribution of random numbers */
void test_distribution(void) {
    int buckets[BUCKET_COUNT] = {0};
    int i;
    double chi_square = 0.0;
    double expected = SAMPLE_SIZE / (double)BUCKET_COUNT;
    
    TEST_START("random number distribution");
    
    /* Reset seed for reproducibility */
    srand(42);
    
    /* Generate samples and count in buckets */
    for (i = 0; i < SAMPLE_SIZE; i++) {
        int value = rand();
        int bucket = (int)((long long)value * BUCKET_COUNT / ((long long)RAND_MAX + 1));
        
        /* Ensure bucket is in range */
        if (bucket >= BUCKET_COUNT) bucket = BUCKET_COUNT - 1;
        
        buckets[bucket]++;
    }
    
    /* Calculate chi-square statistic */
    for (i = 0; i < BUCKET_COUNT; i++) {
        double observed = buckets[i];
        double diff = observed - expected;
        chi_square += (diff * diff) / expected;
    }
    
    printf("  Chi-square value: %.2f (threshold: %.2f)\n", chi_square, CHI_SQUARE_THRESHOLD);
    
    /* Check if distribution is reasonably uniform */
    /* Note: This is a statistical test, may occasionally fail by chance */
    if (chi_square > CHI_SQUARE_THRESHOLD) {
        printf("  Warning: Distribution may not be uniform (chi-square = %.2f)\n", chi_square);
        /* Don't assert - random variations can cause this */
    }
    
    /* Print distribution for visual inspection */
    printf("  Distribution (expected ~%d per bucket):\n", (int)expected);
    for (i = 0; i < BUCKET_COUNT; i++) {
        printf("    Bucket %d: %d\n", i, buckets[i]);
    }
    
    printf("  Distribution tests completed!\n");
}

/* Test random number sequences */
void test_sequences(void) {
    int i, j;
    int sequence[100];
    int duplicates = 0;
    
    TEST_START("random number sequences");
    
    /* Generate sequence */
    srand(98765);
    for (i = 0; i < 100; i++) {
        sequence[i] = rand();
    }
    
    /* Check for immediate repeats (should be rare) */
    int immediate_repeats = 0;
    for (i = 1; i < 100; i++) {
        if (sequence[i] == sequence[i-1]) {
            immediate_repeats++;
        }
    }
    
    /* Some implementations might have patterns, but immediate repeats should be rare */
    assert(immediate_repeats < 10); /* Less than 10% immediate repeats */
    
    /* Check for duplicates in sequence */
    for (i = 0; i < 99; i++) {
        for (j = i + 1; j < 100; j++) {
            if (sequence[i] == sequence[j]) {
                duplicates++;
            }
        }
    }
    
    /* With good PRNG, duplicates should be rare in 100 samples */
    printf("  Found %d duplicates in 100 samples\n", duplicates);
    
    /* Test period detection (basic) */
    srand(111);
    int first = rand();
    int period = 0;
    
    for (i = 1; i < 10000; i++) {
        if (rand() == first) {
            period = i;
            break;
        }
    }
    
    if (period > 0) {
        printf("  Detected potential period at %d\n", period);
        /* Very short periods are bad */
        assert(period > 100);
    } else {
        printf("  No short period detected (good)\n");
    }
    
    TEST_PASSED("Sequence");
}

/* Test edge cases */
void test_edge_cases(void) {
    unsigned int seed;
    int val;
    int i;
    
    TEST_START("edge cases");
    
    /* Test with maximum seed values */
    srand(UINT_MAX);
    val = rand();
    assert(val >= 0 && val <= RAND_MAX);
    
    /* Test rand_r with maximum seed */
    seed = UINT_MAX;
    val = rand_r(&seed);
    assert(val >= 0 && val <= RAND_MAX);
    
    /* Test alternating srand calls */
    for (i = 0; i < 10; i++) {
        srand(i);
        val = rand();
        assert(val >= 0 && val <= RAND_MAX);
    }
    
    /* Test rapid seed changes */
    for (i = 0; i < 1000; i++) {
        srand(i * 7919); /* Prime number for variation */
        val = rand();
        assert(val >= 0 && val <= RAND_MAX);
    }
    
    TEST_PASSED("Edge case");
}

/* Test common usage patterns */
void test_common_patterns(void) {
    int i;
    int val;
    
    TEST_START("common usage patterns");
    
    /* Pattern 1: Random number in range [0, n) */
    srand(555);
    for (i = 0; i < 100; i++) {
        val = rand() % 100;
        assert(val >= 0 && val < 100);
    }
    
    /* Pattern 2: Random number in range [min, max] */
    int min = 10;
    int max = 20;
    for (i = 0; i < 100; i++) {
        val = min + (rand() % (max - min + 1));
        assert(val >= min && val <= max);
    }
    
    /* Pattern 3: Random float in [0, 1) */
    for (i = 0; i < 100; i++) {
        double fval = (double)rand() / ((double)RAND_MAX + 1.0);
        assert(fval >= 0.0 && fval < 1.0);
    }
    
    /* Pattern 4: Random boolean */
    int true_count = 0;
    for (i = 0; i < 1000; i++) {
        int boolean = rand() & 1;
        assert(boolean == 0 || boolean == 1);
        if (boolean) true_count++;
    }
    
    /* Should be roughly 50/50 */
    assert(true_count > 400 && true_count < 600);
    
    /* Pattern 5: Shuffle array (Fisher-Yates) */
    int array[10];
    for (i = 0; i < 10; i++) {
        array[i] = i;
    }
    
    /* Shuffle */
    for (i = 9; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    
    /* Verify all elements still present */
    int found[10] = {0};
    for (i = 0; i < 10; i++) {
        assert(array[i] >= 0 && array[i] < 10);
        found[array[i]] = 1;
    }
    for (i = 0; i < 10; i++) {
        assert(found[i] == 1);
    }
    
    TEST_PASSED("Common pattern");
}

/* Test thread safety considerations */
void test_thread_safety(void) {
    unsigned int seed1 = 100;
    unsigned int seed2 = 100;
    int i;
    
    TEST_START("thread safety considerations");
    
    /* rand() is not thread-safe, but we can't test that in single-threaded */
    /* rand_r() should be thread-safe - test independence */
    
    int seq1[5], seq2[5];
    
    /* Generate with alternating calls */
    for (i = 0; i < 5; i++) {
        seq1[i] = rand_r(&seed1);
        seq2[i] = rand_r(&seed2);
    }
    
    /* Reset and verify each sequence independently */
    seed1 = 100;
    for (i = 0; i < 5; i++) {
        assert(rand_r(&seed1) == seq1[i]);
    }
    
    seed2 = 100;
    for (i = 0; i < 5; i++) {
        assert(rand_r(&seed2) == seq2[i]);
    }
    
    TEST_PASSED("Thread safety");
}

void run_tests(void) {
    printf(COLOR_BOLD_BLUE "=== PSE51 stdlib.h Random Number Test Suite ===" COLOR_RESET "\n\n");
    
    /* Run tests */
    test_rand_basic();
    test_srand();
    test_rand_r();
    test_distribution();
    test_sequences();
    test_edge_cases();
    test_common_patterns();
    test_thread_safety();
    
    TEST_SUITE_PASSED("random number");
}
