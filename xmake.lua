add_rules("mode.debug", "mode.release")
includes("toolchains/*.lua")

local TARGET_DIR = "build/"

local mlibc_config = {
    ["arm32"] = {
        target = "arm32",
        toolchain = "arm-none-eabi",
        define_flags = ""
    },
    ["aarch64"] = {
        target = "aarch64",
        toolchain = "aarch64-none-elf",
        define_flags = "-DARCH_CPU_64BIT"
    }
}

target("mlibc")
    local arch = get_config("arch")
    local config = mlibc_config[arch]
    if not config then 
        config = mlibc_config['arm32']
    end 
    -- Set toolchains
    set_toolchains(config.toolchain)
    -- Set target file type as static library
    set_kind("static")
    -- Strip all information
    set_strip("all")
    -- Set as default target
    set_default(true)
    -- Set target directory
    set_targetdir(TARGET_DIR .. config.target)
    -- Set target filename
    set_filename("libmlibc.a")
    -- Set languages standard
    set_languages("c99", "cxx11")
    -- Set compiler flags, including custom Define
    add_cflags(config.define_flags, "-nostdlib", "-ffreestanding", "-nostdinc", "-Wl,-Map=cc.map", {force = true})
    -- Add all source files
    add_files("src/*.c")
    add_files("src/stdio/*.c")
    add_files("src/stdlib/*.c")
    -- Add header file directory
    add_includedirs("./include", {public = true})
target_end()

option("board")
    set_default("qemu-vexpress-a9")
    set_showmenu(true)
    set_description("Select the board to build for")
    set_values("qemu-arm-r52", "qemu-vexpress-a9", "qemu-virt-aarch64")
option_end()

local testcase_config = {
    ["qemu-arm-r52"] = {
        toolchain = "arm-none-eabi",
        arch = "arm32", 
        envs = {
            DEFINE = " ",
            DEVICE = " -mcpu=cortex-r52 -mthumb ",
            DEBUG  = " -gdwarf-2 "
        },
        flags = {
            cflags = " -O0 ",
            asflags = " ", 
            ldflags = " -nostartfiles -nostdlib -nostdinc -lgcc "
        }
    },
    ["qemu-vexpress-a9"] = {
        toolchain = "arm-none-eabi",
        arch = "arm32", 
        envs = {
            DEFINE = " ",
            DEVICE = " -mcpu=cortex-a9 -mthumb ",
            DEBUG  = " -gdwarf-2 "
        },
        flags = {
            cflags = " -O0 ",
            asflags = " ", 
            ldflags = " -nostartfiles -nostdlib -nostdinc -lgcc "
        }
    },
    ["qemu-virt-aarch64"] = {
        toolchain = "aarch64-none-elf",
        arch = "aarch64", 
        envs = {
            DEFINE = " -mcpu=cortex-a53 -DARCH_CPU_64BIT ",
            DEVICE = " -mstrict-align ",
            DEBUG  = " -gdwarf-2 "
        },
        flags = {
            cflags  = " -O0 ",
            asflags = " ",
            ldflags = " -nostartfiles -nostdlib -nostdinc -lgcc "
        }
    }
}

target("hello")
    local board_path = "testcase/board"
    set_kind("binary")
    set_targetdir("testcase/hello")
    add_imports("core.base.option")
    add_files("testcase/hello/main.c")
    local board = get_config("board")
    local config = testcase_config[board]
    print(board)
    -- Configure toolchain, compile information and parameters
    on_load(function (target)
        -- set toolchains
        target:set("toolchains", config.toolchain)
        target:set("arch", config.arch)
        -- set flags
        local cflags = config.flags.cflags 
                    .. config.envs.DEVICE 
                    .. config.envs.DEFINE 
        local asflags = config.flags.asflags 
                    .. config.envs.DEVICE 
                    .. config.envs.DEFINE 
        local ldflags = config.flags.ldflags
        target:add("cflags", cflags)
        target:add("asflags", asflags)
        target:add("ldflags", "-T " .. path.join(board_path, board, "link.ld") .. ldflags)

        target:add("files", path.join(board_path, board, "startup.s"))
        target:add("includedirs", path.join(board_path, board))

        target:add("deps", "mlibc")
        target:add("linkdirs", path.join("build", config.arch))
        target:add("links", "mlibc")

        target:set("filename", board .. ".elf")
    end)
target_end()