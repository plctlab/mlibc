local project_path = os.projectdir()

option("qemu-board")
    set_default("qemu-vexpress-a9")
    set_showmenu(true)
    set_description("Select the board to build for")
    set_values("qemu-mps3-an536", "qemu-vexpress-a9", "qemu-virt-aarch64", "qemu-virt-riscv64", "qemu-virt-riscv32")
option_end()

local testcase_config = {
    ["qemu-mps3-an536"] = {
        toolchain = "arm-none-eabi",
        arch = "arm",
        crt = "arm",
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
        arch = "arm",
        crt = "arm",
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
        crt = "aarch64",
        envs = {
            DEFINE = " ",
            DEVICE = " -mcpu=cortex-a53 -mstrict-align ",
            DEBUG  = " -gdwarf-2 "
        },
        flags = {
            cflags  = " -O0 ",
            asflags = " -x assembler-with-cpp ",
            ldflags = " -nostartfiles -nostdlib -nostdinc -lgcc "
        }
    },
    ["qemu-virt-riscv64"] = {
        toolchain = "riscv64-unknown-elf",
        arch = "riscv64",
        crt = "riscv",
        envs = {
            DEFINE = " -D__UNDEF_FFS__ ",
            DEVICE = " -mcmodel=medany -mstrict-align ",
            DEBUG  = " -gdwarf-2 "
        },
        flags = {
            cflags  = " -O0 ",
            asflags = " ",
            ldflags = " -nostartfiles -nostdlib -nostdinc -lgcc "
        }
    },
    ["qemu-virt-riscv32"] = {
        toolchain = "riscv32-unknown-elf",
        arch = "riscv32",
        crt = "riscv", 
        envs = {
            DEFINE = " ",
            DEVICE = " -mcmodel=medany -mstrict-align -march=rv32imac -mabi=ilp32",
            DEBUG  = " -gdwarf-2 "
        },
        flags = {
            cflags  = " -O0 ",
            asflags = " ",
            ldflags = " -nostartfiles -nostdlib -nostdinc -lgcc "
        }
    }
}

target("qemu-hello")
    local board_path = "helloworld/qemu"
    local board = get_config("qemu-board")
    local config = testcase_config[board]
    set_kind("binary")
    add_imports("core.base.option")
    add_files(path.join(project_path, "helloworld/main.c"))
    -- debug
    add_files(path.join(project_path, "src/dummy/*.c"))
    add_files(path.join(project_path, "src/misc/*.c"))
    add_files(path.join(project_path, "src/stdio/*.c"))
    add_files(path.join(project_path, "src/stdlib/*.c"))
    add_files(path.join(project_path, "src/time/*.c"))
    add_includedirs(path.join(project_path, "include"))
    -- debug
    -- Configure toolchain, compile information and parameters
    on_load(function (target)
        target:set("arch", "")
        target:set("plat", "")
        target:set("mode", "debug")
        -- set toolchains
        target:set("toolchains", config.toolchain)
        target:set("arch", config.arch)
        -- set flags
        local cflags = config.flags.cflags 
                    .. config.envs.DEVICE 
                    .. config.envs.DEFINE 
                    .. config.envs.DEBUG
        local asflags = config.flags.asflags 
                    .. config.envs.DEVICE 
                    .. config.envs.DEFINE 
                    .. config.envs.DEBUG
        local ldflags = config.flags.ldflags
        
        target:add("cflags", cflags)
        target:add("asflags", asflags)
        target:add("ldflags", "-T " .. path.join(board_path, board, "link.ld") .. ldflags, {force = true})

        target:add("includedirs", path.join(board_path, board))
        target:add("files", path.join("crt", config.crt, "crt0.c"))

        target:set("targetdir", path.join(board_path, board))
        target:set("filename", board .. ".elf")
    end)
target_end()