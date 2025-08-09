# Makefile for stdlib tests (multiple test files)

include ../common/Makefile.common

# Test programs
TESTS = test_memory \
        test_string_conversion \
        test_random \
        test_integer_math \
        test_program_control \
        test_environment_utils

PC_TARGETS = $(TESTS:%=$(TEST_RESULTS_DIR)/%)
EMBEDDED_TARGETS = $(TESTS:%=%_embedded.elf)

# Additional libraries for PC build
PC_LDFLAGS += -lm -pthread

# Default target
all: pc

pc: $(TEST_RESULTS_DIR) $(PC_TARGETS)

embedded: $(BUILD_DIR) $(EMBEDDED_TARGETS)

# Build rules for each PC test with main.c
$(TEST_RESULTS_DIR)/%: %.c $(COMMON_DIR)/main.c
	@echo "Building $@..."
	$(PC_CC) $(PC_CFLAGS) -o $@ $^ $(PC_LDFLAGS)
	@echo "$(GREEN)✓ PC test $@ built$(NC)"

# Embedded build rules for each test
$(BUILD_DIR)/embedded_startup.o: $(COMMON_DIR)/embedded_startup.c
	@echo "Building embedded startup..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/main.o: $(COMMON_DIR)/main.c
	@echo "Building main.o..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@echo "Building $@..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

%_embedded.elf: $(BUILD_DIR)/embedded_startup.o $(BUILD_DIR)/%.o $(BUILD_DIR)/main.o
	@echo "Linking $@..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -o $@ $^ $(EMBEDDED_LDFLAGS)
	@echo "$(GREEN)✓ Embedded test $@ built$(NC)"

# Run all PC tests
test: $(PC_TARGETS)
	@echo "Running stdlib tests..."
	@for test in $(PC_TARGETS); do \
		echo "Running $$test..."; \
		$$test || true; \
	done

# Run specific embedded test
run: test_memory_embedded.elf
	@echo "Running embedded test in QEMU ($(EMBEDDED_ARCH))..."
	@$(QEMU_SYSTEM) $(QEMU_ARGS) -kernel $<

# Clean
clean:
	@rm -rf $(TEST_RESULTS_DIR) $(BUILD_DIR)
	@rm -f *.o *.out *.elf
	@echo "$(GREEN)✓ stdlib clean completed$(NC)"

.PHONY: all pc embedded test run clean