@echo off
qemu-system-riscv64 -machine virt -nographic -m 256M -kernel qemu-virt-riscv64.elf ^
-device virtio-serial-device
