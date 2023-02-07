
PATH = "E:\toolchain\riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-w64-mingw32"

toolchain("riscv64-unknown-elf")
    set_kind("standalone")
    set_bindir("E:/toolchain/riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-w64-mingw32/bin")
    set_toolset("cc", "gcc@$PATH\bin\riscv64-unknown-elf-gcc.exe")
    set_toolset("cxx", "gcc@$PATH\bin\riscv64-unknown-elf-g++.exe")
    set_toolset("ld", "gcc@$PATH\bin\riscv64-unknown-elf-ld.exe")
    --set_toolset("sh", "gcc@$PATH\bin\riscv64-unknown-elf-gcc-sh.exe")
    set_toolset("ar", "gcc@$PATH\bin\riscv64-unknown-elf-gcc-ar.exe")
    set_toolset("ex", "gcc@$PATH\bin\riscv64-unknown-elf-gcc-ar.exe")
    set_toolset("strip", "gcc@$PATH\bin\riscv64-unknown-elf-strip.exe")
    set_toolset("mm", "gcc@$PATH\bin\riscv64-unknown-elf-gcc.exe")
    set_toolset("mxx", "gcc@$PATH\bin\riscv64-unknown-elf-gcc.exe", "gcc@$PATH\bin\riscv64-unknown-elf-g++.exe")
    set_toolset("as", "gcc@$PATH\bin\riscv64-unknown-elf-gcc.exe")
toolchain_end()


