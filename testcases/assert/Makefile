# Makefile for PSE51 assert.h test suite

CC ?= gcc
CFLAGS = -Wall -Wextra -g -std=c99 -D_POSIX_C_SOURCE=200112L

# Test programs
TESTS = test_assert test_assert_ndebug

# Output directory
TEST_RESULTS_DIR = test_results

# All test executables
TEST_EXES = $(TEST_RESULTS_DIR)/test_assert $(TEST_RESULTS_DIR)/test_assert_ndebug

# Default target
all: $(TEST_RESULTS_DIR) $(TEST_EXES)

# Create output directory
$(TEST_RESULTS_DIR):
	@mkdir -p $(TEST_RESULTS_DIR)

# Build normal assert test
$(TEST_RESULTS_DIR)/test_assert: test_assert.c
	$(CC) $(CFLAGS) -o $@ $<

# Build NDEBUG version
$(TEST_RESULTS_DIR)/test_assert_ndebug: test_assert.c
	$(CC) $(CFLAGS) -DNDEBUG -o $@ $<

# Run all tests
test: all
	@echo "Running assert.h tests..."
	@echo "Testing with assertions enabled..."
	@$(TEST_RESULTS_DIR)/test_assert && echo "✓ Assert tests passed" || echo "✗ Assert tests failed"
	@echo ""
	@echo "Testing with NDEBUG (assertions disabled)..."
	@$(TEST_RESULTS_DIR)/test_assert_ndebug && echo "✓ NDEBUG tests passed" || echo "✗ NDEBUG tests failed"

# Clean build artifacts
clean:
	rm -rf $(TEST_RESULTS_DIR)

# Help target
help:
	@echo "PSE51 assert.h Test Suite"
	@echo ""
	@echo "Usage:"
	@echo "  make      - Build tests"
	@echo "  make test - Build and run tests"
	@echo "  make clean - Clean build artifacts"

.PHONY: all test clean help