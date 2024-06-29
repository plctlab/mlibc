PATH = "D:\\My\\plugin\\gcc-arm-none-eabi-10.3-2021.10-win32"
BIN_DIR = PATH .. "\\bin"

-- 设置交叉编译工具链
toolchain("arm-none-eabi")
    set_kind("standalone")
    set_sdkdir(PATH)
    set_bindir(BIN_DIR)
toolchain_end()
