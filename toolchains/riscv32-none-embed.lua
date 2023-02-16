
-- PATH = "D:\RT-ThreadStudio\repo\Extract\ToolChain_Support_Packages\WCH\RISC-V-GCC-WCH\8.2.0"
PATH = "D:/RT-ThreadStudio/repo/Extract/ToolChain_Support_Packages/WCH/RISC-V-GCC-WCH/8.2.0"
PREFIX = "riscv-none-embed-"

toolchain("riscv-none-embed")
    set_kind("standalone")
    -- set_bindir("$(PATH)/bin")
    -- set_sdkdir("$(PATH)")
    set_toolset("cc", "$(PATH)\bin\riscv-none-embed-gcc.exe")
    set_toolset("cxx", "$(PATH)\bin\riscv-none-embed-g++.exe")
    set_toolset("ld", "$(PATH)\bin\riscv-none-embed-ld.exe")
    --set_toolset("sh", "$(PATH)\bin\riscv-none-embed-gcc-sh.exe")
    set_toolset("ar", "$(PATH)\bin\riscv-none-embed-gcc-ar.exe")
    set_toolset("ex", "$(PATH)\bin\riscv-none-embed-gcc-ar.exe")
    set_toolset("strip", "$(PATH)\bin\riscv-none-embed-strip.exe")
    set_toolset("mm", "$(PATH)\bin\riscv-none-embed-gcc.exe")
    set_toolset("mxx", "$(PATH)\bin\riscv-none-embed-gcc.exe", "$(PATH)\bin\riscv-none-embed-g++.exe")
    set_toolset("as", "$(PATH)\bin\riscv-none-embed-gcc.exe")

toolchain_end()


