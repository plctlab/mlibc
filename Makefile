ifeq ($(OS),Windows_NT)
	SHELL := cmd.exe
endif

PROJECT_PATH := .

# Set default toolchain if MLIBC_TOOLCHAIN is not set
ifeq ($(MLIBC_TOOLCHAIN),)
    MLIBC_TOOLCHAIN := $(PROJECT_PATH)/toolchain/arm-linux-eabi_for_x86_64-pc-linux-gnu/bin/arm-linux-eabi-
endif

CC := $(MLIBC_TOOLCHAIN)gcc
AR := $(MLIBC_TOOLCHAIN)ar

# Select architecture (you can set this manually or through environment variables)
ARCH ?= arm

# Configuration for arch
ifeq ($(ARCH), arm)
ARCH_FLAGS :=
DEFINE_FLAGS :=
endif

ifeq ($(ARCH), aarch64)
ARCH_FLAGS := -mstrict-align
DEFINE_FLAGS :=
endif

ifeq ($(ARCH), riscv32)
ARCH_FLAGS := -mcmodel=medany -march=rv32imac
DEFINE_FLAGS :=
endif

ifeq ($(ARCH), riscv64)
ARCH_FLAGS := -mcmodel=medany
DEFINE_FLAGS :=
endif

# flags
CFLAGS := $(DEFINE_FLAGS) $(ARCH_FLAGS) -nostdlib -ffreestanding -nostdinc -Wl,-Map=cc.map
INCLUDES := -I $(PROJECT_PATH)/include

# Source files
SRC_FILES := $(wildcard $(PROJECT_PATH)/src/dummy/*.c) \
             $(wildcard $(PROJECT_PATH)/src/misc/*.c) \
             $(wildcard $(PROJECT_PATH)/src/stdio/*.c) \
             $(wildcard $(PROJECT_PATH)/src/stdlib/*.c) \
             $(wildcard $(PROJECT_PATH)/src/time/*.c)

CRT_FILES := $(wildcard $(PROJECT_PATH)/crt/$(ARCH)/*.c) \
             $(wildcard $(PROJECT_PATH)/crt/$(ARCH)/*.s)

# Target settings
TARGET := libmlibc.a
TARGET_DIR_ARCH := $(PROJECT_PATH)/build/$(ARCH)
LIB_PATH := $(PROJECT_PATH)/mlibc/lib

# Object files directory
OBJ_DIR := $(TARGET_DIR_ARCH)/obj
CRTOBJ_DIR := $(TARGET_DIR_ARCH)/crtobj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))
CRTOBJ_FILE := $(patsubst %.c,$(CRTOBJ_DIR)/%.o,$(notdir $(CRT_FILES)))
CRTOBJ_FILES := $(patsubst %.S,$(CRTOBJ_DIR)/%.o,$(CRTOBJ_FILE))

# Include QEMU hello world build configuration
include $(PROJECT_PATH)/mkconfigs/qemu/qemu-hello.mk

# Build rules
.PHONY: all
all : mlibc crt

.PHONY: mlibc
mlibc: creat_mdir $(TARGET_DIR_ARCH)/$(TARGET)
# Compile each .c file into .o in the obj directory
$(OBJ_DIR)/%.o: $(PROJECT_PATH)/src/*/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET_DIR_ARCH)/$(TARGET): $(OBJ_FILES) 
	$(AR) rcs $@ $(OBJ_FILES)

.PHONY: creat_mdir
creat_mdir:
ifeq ($(OS),Windows_NT)
	if not exist $(subst /,\,$(OBJ_DIR)) @mkdir $(subst /,\,$(OBJ_DIR))	
	if not exist $(subst /,\,$(CRTOBJ_DIR)) @mkdir $(subst /,\,$(CRTOBJ_DIR))
else
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(CRTOBJ_DIR)
endif

.PHONY: crt
crt: creat_cdir $(CRTOBJ_FILES)

$(CRTOBJ_DIR)/%.o: $(PROJECT_PATH)/crt/$(ARCH)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(CRTOBJ_DIR)/%.o: $(PROJECT_PATH)/crt/$(ARCH)/%.s
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: creat_cdir
creat_cdir:
ifeq ($(OS),Windows_NT)
	if not exist $(subst /,\,$(CRTOBJ_DIR)) @mkdir $(subst /,\,$(CRTOBJ_DIR))
else
	@mkdir -p $(CRTOBJ_DIR)
endif

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	@rmdir /s/q $(subst /,\,$(PROJECT_PATH))\build
else
	@rm -rf $(PROJECT_PATH)/build
	@echo ${CRTOBJ_FILES}
	@echo ${CRTOBJ_FILE}
endif
