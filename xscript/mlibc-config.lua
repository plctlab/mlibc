local project_path = os.projectdir()
local TARGET_DIR = "build/"
add_rules("mode.debug", "mode.release")

option("mlibc-arch")
    set_default("arm")
    set_showmenu(true)
    set_description("Select the crt0 to build for")
    set_values("arm", "riscv32", "riscv64", "aarch64")
option_end()

local mlibc_config = {
    ["arm"] = {
        target = "arm",
        toolchain = "arm-none-eabi",
        arch_flags = " ",
        define_flags = " "
    },
    ["aarch64"] = {
        target = "aarch64",
        toolchain = "aarch64-none-elf",
        arch_flags = " -mstrict-align ",
        define_flags = " "
    },
    ["riscv64"] = {
        target = "riscv64",
        toolchain = "riscv64-unknown-elf",
        arch_flags = " -mcmodel=medany ",
        define_flags = " "
    },
    ["riscv32"] = {
        target = "riscv32",
        toolchain = "riscv32-unknown-elf",
        arch_flags = " -mcmodel=medany -march=rv32imac ",
        define_flags = " "
    }
}

target("mlibc")
    local arch = get_config("mlibc-arch")
    local config = mlibc_config[arch]
    -- Set target file type as static library
    set_kind("static")
    -- Strip all information
    set_strip("all")
    -- Set as default target
    set_default(true)
    -- Set target filename
    set_filename("libmlibc.a")
    -- Set languages standard
    set_languages("c99", "cxx11")
    -- Add all source files
    add_files(path.join(project_path, "src/dummy/*.c")) 
    add_files(path.join(project_path, "src/misc/*.c"))
    add_files(path.join(project_path, "src/stdio/*.c"))
    add_files(path.join(project_path, "src/stdlib/*.c"))
    add_files(path.join(project_path, "src/time/*.c"))
    -- Add header file directory
    add_includedirs(path.join(project_path, "./include"), {public = true})
    on_load(function (target)
        target:set("arch", "")
        target:set("plat", "")
        -- Set toolchains
        target:set("toolchains", config.toolchain)
        -- Set target directory
        target:set("targetdir", TARGET_DIR .. config.target)
        -- Set compiler flags, including custom Define
        target:add("cflags", config.define_flags, config.arch_flags, "-nostdlib", "-ffreestanding", "-nostdinc", "-Wl,-Map=cc.map", {force = true})
    end)

    after_build(function (target)
        os.mkdir("mlibc/lib")
        os.cp(path.join(project_path, "include"), path.join(project_path, "mlibc"))
        os.cp(path.join(project_path, TARGET_DIR, config.target, "libmlibc.a"), "mlibc/lib/libc.a")
    end)
target_end()