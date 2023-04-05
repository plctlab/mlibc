add_rules("mode.debug", "mode.release")
includes("toolchains/*.lua")

TARGET_DIR  = "./mlibc"

DEVICE = ' -mcmodel=medany -march=rv64imafdc -mabi=lp64 '

--set target
target("mlibc")
    -- set target file type
    set_kind("static")
    --strip all infomation
    set_strip("all")
    --set default target
    set_default(true)
    -- set target dir
    set_targetdir(TARGET_DIR)
    -- --set target filename
    -- set_filename("mlibc.a")
    --set optimize O1
    set_optimize("fast")
    --set languages standard
    set_languages("c99", "cxx11")
    -- set compiler cflags for device
    add_cflags(DEVICE, {force = true})
    -- set compiler cflags for no standard library
    add_cflags("-nostdlib","-ffreestanding","-nostdinc", "-Wl,-Map=cc.map",{force = true})
    -- add all files
    add_files("src/*.c")
    -- add headfile dir
    add_includedirs("./include",{public = true})

    add_includedirs("./arch/riscv64",{public = true})

    set_toolchains("riscv64-unknown-elf")
    
    after_link(function (target)
    os.cp("./include/*.h", "./mlibc")
    os.cp("./arch/riscv64*.h", "./mlibc")
    os.cp("./mlibc", "../../mlibc-qemu-riscv64-test")
    end)
    
target_end() 
    


-- target("include")
--     set_kind("headeronly")

--     add_files("include/*.h", "./arch/riscv64",{public = true})
--     add_files("./arch/riscv64/*.h",{public = true})

--     add_cflags(DEVICE, {force = true})
--     add_cflags("-nostdlib","-ffreestanding","-nostdinc", {force = true})
--     set_toolchains("riscv64-unknown-elf")
--     -- set_targetdir("./../../GIT/riscv-qemu/project/mlibc")
--     set_targetdir("./")

--     set_filename("mlibc.h")
-- target_end()
--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

