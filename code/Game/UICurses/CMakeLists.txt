fips_begin_module(UICurses)
    fips_vs_warning_level(3)
    fips_vs_disable_warnings(4005 4996)

    fips_include_directories(.)

    fips_files_ex(. *.cc GROUP "Source Files")
    fips_files_ex(. *.h GROUP "Header Files")

    if(FIPS_WINDOWS)
        fips_deps(ECS PDCurses)
    else()
        fips_deps(ECS ncurses)
    endif()
fips_end_module()