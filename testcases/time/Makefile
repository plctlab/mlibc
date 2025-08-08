# Makefile for PSE51 time.h test suite

CC ?= gcc
CFLAGS = -Wall -Wextra -g -std=c99 -D_POSIX_C_SOURCE=200112L
LDFLAGS = -lrt -lpthread

# Test programs
TESTS = test_time

# Output directory
TEST_RESULTS_DIR = test_results

# All test executables
TEST_EXES = $(TESTS:%=$(TEST_RESULTS_DIR)/%)

# Default target
all: $(TEST_RESULTS_DIR) $(TEST_EXES)

# Create output directory
$(TEST_RESULTS_DIR):
	@mkdir -p $(TEST_RESULTS_DIR)

# Build rules
$(TEST_RESULTS_DIR)/test_time: test_time.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run all tests
test: all
	@echo "Running all time.h tests..."
	@$(TEST_RESULTS_DIR)/test_time && echo "✓ All tests passed" || echo "✗ Tests failed"

# Clean build artifacts
clean:
	rm -rf $(TEST_RESULTS_DIR)

# Help target
help:
	@echo "PSE51 time.h Test Suite"
	@echo ""
	@echo "Usage:"
	@echo "  make      - Build tests"
	@echo "  make test - Build and run tests"
	@echo "  make clean - Clean build artifacts"

.PHONY: all test clean help