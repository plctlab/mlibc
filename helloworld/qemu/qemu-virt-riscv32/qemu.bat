@echo off
qemu-system-riscv32 -machine virt -nographic -m 256M -kernel qemu-virt-riscv32.elf ^
-device virtio-serial-device
