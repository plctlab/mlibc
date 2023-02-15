
PATH = "E:\toolchain\riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-w64-mingw32"

toolchain("riscv64-unknown-elf")
    set_kind("standalone")
    set_bindir("E:/toolchain/riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-w64-mingw32/bin")
    set_toolset("cc", "$PATH\bin\riscv64-unknown-elf-gcc.exe")
    set_toolset("cxx", "$PATH\bin\riscv64-unknown-elf-g++.exe")
    set_toolset("ld", "$PATH\bin\riscv64-unknown-elf-ld.exe")
    --set_toolset("sh", "$PATH\bin\riscv64-unknown-elf-gcc-sh.exe")
    set_toolset("ar", "$PATH\bin\riscv64-unknown-elf-gcc-ar.exe")
    set_toolset("ex", "$PATH\bin\riscv64-unknown-elf-gcc-ar.exe")
    set_toolset("strip", "$PATH\bin\riscv64-unknown-elf-strip.exe")
    set_toolset("mm", "$PATH\bin\riscv64-unknown-elf-gcc.exe")
    set_toolset("mxx", "$PATH\bin\riscv64-unknown-elf-gcc.exe", "$PATH\bin\riscv64-unknown-elf-g++.exe")
    set_toolset("as", "$PATH\bin\riscv64-unknown-elf-gcc.exe")

toolchain_end()


