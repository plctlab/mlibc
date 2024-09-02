@echo off
qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel qemu-virt-aarch64.elf -serial stdio -m 512
