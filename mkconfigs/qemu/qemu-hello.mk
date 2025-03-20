QEMU_BOARD ?= qemu-vexpress-a9

# Configuration for qemu 
ifeq ($(QEMU_BOARD), qemu-mps3-an536)
include $(PROJECT_PATH)/mkconfigs/qemu/qemu-mps3-an536.mk
endif

ifeq ($(QEMU_BOARD), qemu-vexpress-a9)
include $(PROJECT_PATH)/mkconfigs/qemu/qemu-vexpress-a9.mk
endif

ifeq ($(QEMU_BOARD), qemu-virt-aarch64)
include $(PROJECT_PATH)/mkconfigs/qemu/qemu-virt-aarch64.mk
endif

ifeq ($(QEMU_BOARD), qemu-virt-riscv32)
include $(PROJECT_PATH)/mkconfigs/qemu/qemu-virt-riscv32.mk
endif

ifeq ($(QEMU_BOARD), qemu-virt-riscv64)
include $(PROJECT_PATH)/mkconfigs/qemu/qemu-virt-riscv64.mk
endif

BOARD_PATH := $(PROJECT_PATH)/helloworld
QEMU_SRCS := $(SRC_FILES)  $(BOARD_PATH)/main.c
QEMU_INC := $(INCLUDES) -I$(BOARD_PATH)/qemu/$(QEMU_BOARD)
QEMU_CRTOBJ_DIR = $(TARGET_DIR_ARCH)/qemu/$(QEMU_BOARD)/crtobj
QEMU_OBJ_DIR = $(TARGET_DIR_ARCH)/qemu/$(QEMU_BOARD)/obj
QEMU_CRTOBJ_FILES := $(QEMU_CRTOBJ_DIR)/crt0.o
QEMU_OBJ_FILES := $(patsubst %.c,$(QEMU_OBJ_DIR)/%.o,$(notdir $(QEMU_SRCS)))

QCFLAGS := $(QCFLAGS) $(DEVICE) $(DEFINE) $(DEBUG)
ASFLAGS := $(ASFLAGS) $(DEVICE) $(DEFINE) $(DEBUG)
LDFLAGS := -T $(BOARD_PATH)/qemu/$(QEMU_BOARD)/link.ld $(LDFLAGS)

TARGET := $(QEMU_BOARD).elf

$(QEMU_CRTOBJ_DIR)/%.o: $(PROJECT_PATH)/crt/$(ARCH)/%.c
	$(CC) -c $(QEMU_INC) $(QCFLAGS) -o $@ $<

$(QEMU_OBJ_DIR)/%.o: $(PROJECT_PATH)/src/*/%.c
	$(CC) -c $(QEMU_INC) $(QCFLAGS) -o $@ $<

$(QEMU_OBJ_DIR)/%.o: $(BOARD_PATH)/%.c
	$(CC) -c $(QEMU_INC) $(QCFLAGS) -o $@ $<

.PHONY:qemu-hello
qemu-hello: creat_qdir $(BOARD_PATH)/qemu/$(QEMU_BOARD)/$(TARGET)

.PHONY: creat_qdir
creat_qdir:
ifeq ($(OS),Windows_NT)
	if not exist $(subst /,\,$(QEMU_CRTOBJ_DIR)) @mkdir $(subst /,\,$(QEMU_CRTOBJ_DIR))
	if not exist $(subst /,\,$(QEMU_OBJ_DIR)) @mkdir $(subst /,\,$(QEMU_OBJ_DIR))    
else
	@mkdir -p $(QEMU_CRTOBJ_DIR)
	@mkdir -p $(QEMU_OBJ_DIR)
endif

$(BOARD_PATH)/qemu/$(QEMU_BOARD)/$(TARGET): $(QEMU_CRTOBJ_FILES) $(QEMU_OBJ_FILES)
	$(CC) -o $@ $(QEMU_OBJ_FILES) $(QEMU_CRTOBJ_FILES) $(LDFLAGS)
