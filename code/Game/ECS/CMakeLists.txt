fips_begin_module(ECS)
    fips_include_directories(.)

    if (FIPS_WINDOWS)
        fips_include_directories(../../Ext/PDCurses)
        fips_deps(PDCurses)
    else()
        fips_include_directories(../../Ext/ncurses-6.2/include)
        link_directories(../../Ext/ncurses-6.2/lib)
        fips_libs(
            libncurses_g.a
            libncurses.a
            libncurses++_g.a
            libncurses++.a
        )
    endif()

    fips_files_ex(Entity *.* GROUP "Entity")
    fips_files_ex(Component *.* GROUP "Component")
    fips_files_ex(System *.* GROUP "System")
    fips_files_ex(World *.* GROUP "World")
    
    fips_dir(.)
    fips_files(
        Global.h
        Global.cc
    )

    fips_deps(Terrain)
fips_end_module()

fips_begin_unittest(ECS)
    fips_vs_warning_level(3)
    fips_vs_disable_warnings(4819 4996)  # 为了中文注释 cp936
    
    fips_dir(UnitTests)
    if(FIPS_WINDOWS)
    endif()
    fips_files(
        MainLoopTest.cc
    )
    fips_deps(ECS)
fips_end_unittest()