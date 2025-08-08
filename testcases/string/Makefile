# Makefile for PSE51 string.h test suite

CC ?= gcc
CFLAGS = -Wall -Wextra -g -std=c99 -D_POSIX_C_SOURCE=200112L
LDFLAGS = -pthread

# Test programs
TESTS = test_memory_ops \
        test_string_copy \
        test_string_concat \
        test_string_search \
        test_string_compare \
        test_string_misc

# Output directory
TEST_RESULTS_DIR = test_results

# All test executables
TEST_EXES = $(TESTS:%=$(TEST_RESULTS_DIR)/%)

# Default target
all: $(TEST_RESULTS_DIR) $(TEST_EXES)

# Create output directory
$(TEST_RESULTS_DIR):
	@mkdir -p $(TEST_RESULTS_DIR)

# Build rules for each test
$(TEST_RESULTS_DIR)/test_memory_ops: test_memory_ops.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(TEST_RESULTS_DIR)/test_string_copy: test_string_copy.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(TEST_RESULTS_DIR)/test_string_concat: test_string_concat.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(TEST_RESULTS_DIR)/test_string_search: test_string_search.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(TEST_RESULTS_DIR)/test_string_compare: test_string_compare.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(TEST_RESULTS_DIR)/test_string_misc: test_string_misc.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Run all tests
test: all
	@echo "Running all string.h tests..."
	@for test in $(TESTS); do \
		echo "Running $$test..."; \
		$(TEST_RESULTS_DIR)/$$test > $(TEST_RESULTS_DIR)/$$test.log 2>&1 && \
		echo "✓ $$test passed" || echo "✗ $$test failed"; \
	done

# Run individual tests
run-%: $(TEST_RESULTS_DIR)/%
	@echo "Running $*..."
	@$(TEST_RESULTS_DIR)/$* && echo "✓ $* passed" || echo "✗ $* failed"

# Clean build artifacts
clean:
	rm -rf $(TEST_RESULTS_DIR)
	rm -f *.tmp

# Show available tests
list:
	@echo "Available tests:"
	@for test in $(TESTS); do echo "  - $$test"; done

# Help target
help:
	@echo "PSE51 string.h Test Suite"
	@echo ""
	@echo "Usage:"
	@echo "  make            - Build all tests"
	@echo "  make test       - Build and run all tests"
	@echo "  make run-<test> - Run a specific test"
	@echo "  make clean      - Clean build artifacts"
	@echo "  make list       - List available tests"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make run-test_memory_ops"
	@echo "  make run-test_string_copy"

.PHONY: all test clean list help $(TESTS:%=run-%)