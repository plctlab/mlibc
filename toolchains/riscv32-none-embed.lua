PATH = "D:\\RT-ThreadStudio\\repo\\Extract\\ToolChain_Support_Packages\\WCH\\RISC-V-GCC-WCH\\8.2.0"
BIN_DIR = "$(PATH)\\bin"
PREFIX = "riscv-none-embed-"

toolchain("riscv-none-embed")
    set_kind("standalone")
    
    -- set_bindir("$(PATH)\\bin")
    -- set_sdkdir("$(PATH)")
    set_toolset("cc", "$(BIN_DIR)\\$(PREFIX)gcc.exe")
    set_toolset("cxx", "$(BIN_DIR)\\$(PREFIX)g++.exe")
    set_toolset("ld", "$(BIN_DIR)\\$(PREFIX)ld.exe")
    --set_toolset("sh", "$(BIN_DIR)$(PREFIX)gcc-sh.exe")
    set_toolset("ar", "$(BIN_DIR)\\$(PREFIX)gcc-ar.exe")
    set_toolset("ex", "$(BIN_DIR)\\$(PREFIX)gcc-ar.exe")
    set_toolset("strip", "$(BIN_DIR)\\$(PREFIX)strip.exe")
    set_toolset("mm", "$(BIN_DIR)\\$(PREFIX)gcc.exe")
    set_toolset("mxx", "$(BIN_DIR)\\$(PREFIX)gcc.exe", "$(BIN_DIR)\\$(PREFIX)g++.exe")
    set_toolset("as", "$(BIN_DIR)\\$(PREFIX)gcc.exe")

toolchain_end()


