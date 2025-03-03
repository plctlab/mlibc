add_requires("cmocka")

local project_path = os.projectdir()

-- 定义测试范围选项
option("test-cover")
    set_default("all")
    set_showmenu(true)
    set_description("Select test coverage areas (comma-separated: all,ctype,stdlib,string)")
    set_values("all", "ctype")
option_end()

target("mlibc-test")
    set_kind("binary")
    add_packages("cmocka")
    add_includedirs(path.join(project_path, "test/testcase"))
    add_deps("mlibc_hosted")

    -- 根据测试覆盖范围选择源文件
    local test_cover = get_config("test-cover") or "all"

    -- 统一处理'all'特殊值
    if test_cover == "all" then
        add_files(path.join(project_path, "test/testcase/**.c"))
    else
        add_files(path.join(path.join(project_path, "test/testcase/", test_cover , "/*.c")))
    end 

    -- 生成链接器映射文件
    add_ldflags("-W",{force = true})
    
    -- GCC/Clang 编译器标志
    add_cflags("-Wno-stringop-overflow", "-Wno-unused-parameter", 
                "-Wno-nonnull-compare", "-Wno-nonnull", 
                "-Wno-stringop-truncation", "-Wno-unknown-pragmas", 
                {force = true})

