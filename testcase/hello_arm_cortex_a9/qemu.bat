@echo off
qemu-system-arm --version
qemu-system-arm -M vexpress-a9 -kernel cortex-a9.elf -serial stdio -m 512
