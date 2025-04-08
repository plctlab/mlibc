@echo off
qemu-system-aarch64 -M virt -cpu cortex-a53 -kernel ../../../build/aarch64/qemu/qemu-virt-aarch64/qemu-virt-aarch64.elf -serial stdio -m 512