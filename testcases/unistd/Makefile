# Makefile for TEST_NAME tests

TEST_NAME := $(notdir $(CURDIR))
include ../common/Makefile.common

# Test source files
TEST_SRC = test_$(TEST_NAME).c
MAIN_SRC = $(COMMON_DIR)/main.c

# PC build
PC_TARGET = $(TEST_RESULTS_DIR)/test_$(TEST_NAME)

# Embedded build  
EMBEDDED_TARGET = test_$(TEST_NAME)_embedded.elf

# Embedded object files
EMBEDDED_OBJS = $(BUILD_DIR)/embedded_startup.o \
                $(BUILD_DIR)/test_$(TEST_NAME).o \
                $(BUILD_DIR)/main.o

# Default target
all: pc

pc: $(TEST_RESULTS_DIR) $(PC_TARGET)

embedded: $(BUILD_DIR) $(EMBEDDED_TARGET)

# PC test build
$(PC_TARGET): $(TEST_SRC) $(MAIN_SRC)
	@echo "Building PC test..."
	$(PC_CC) $(PC_CFLAGS) -o $@ $^ $(PC_LDFLAGS)
	@echo "$(GREEN)✓ PC test built$(NC)"

# Embedded test build
$(BUILD_DIR)/embedded_startup.o: $(COMMON_DIR)/embedded_startup.c
	@echo "Building embedded startup..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/test_$(TEST_NAME).o: test_$(TEST_NAME).c
	@echo "Building test_$(TEST_NAME).o..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/main.o: $(MAIN_SRC)
	@echo "Building main.o..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -c -o $@ $<

$(EMBEDDED_TARGET): $(EMBEDDED_OBJS)
	@echo "Linking embedded test..."
	$(EMBEDDED_CC) $(EMBEDDED_CFLAGS) -o $@ $^ $(EMBEDDED_LDFLAGS)
	@echo "$(GREEN)✓ Embedded test built$(NC)"

# Run targets
test: $(PC_TARGET)
	@echo "Running PC test..."
	@./$(PC_TARGET)

run: $(EMBEDDED_TARGET)
	@echo "Running embedded test in QEMU ($(EMBEDDED_ARCH))..."
	@$(QEMU_SYSTEM) $(QEMU_ARGS) -kernel $(EMBEDDED_TARGET)

.PHONY: all pc embedded test run clean