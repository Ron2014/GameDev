fips_begin_module(ECS)
    fips_include_directories(.)

    if (FIPS_WINDOWS)
        fips_include_directories(../../Ext/PDCurses)
    else()
        # # if you want to use lib directly in linux
        # # enter ncurses-6.2 do these command
        # # $./configure && sudo make
        # # and then use the library like this

        # fips_include_directories(../../Ext/ncurses-6.2/include)
        # link_directories(../../Ext/ncurses-6.2/lib)
        # fips_libs(
        #     libncurses_g.a
        #     libncurses.a
        #     libncurses++_g.a
        #     libncurses++.a
        # )

        fips_include_directories(../../Ext/ncurses-6.2/include)
        fips_include_directories(../../Ext/ncurses-6.2/ncurses)
    endif()
    
    fips_dir(.)
    fips_files(
        Global.h
        Global.cc
    )

    fips_files_ex(Terrain *.* GROUP "Terrain")
    fips_files_ex(World *.* GROUP "World")
    fips_files_ex(Entity *.* GROUP "Entity")
    fips_files_ex(Component *.* GROUP "Component")
    fips_files_ex(System *.* GROUP "System")

    if(FIPS_WINDOWS)
        fips_deps(Core PDCurses)
    else()
        fips_deps(Core ncurses)
    endif()
    
fips_end_module()

fips_begin_unittest(MainLoop)
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

fips_begin_unittest(TerrainLoader)
    fips_vs_warning_level(3)
    fips_vs_disable_warnings(4819 4996)  # 为了中文注释 cp936
    
    fips_dir(UnitTests)
    if(FIPS_WINDOWS)
    endif()
    fips_files(
        TerrainLoader.cc
    )
    fips_deps(ECS)
fips_end_unittest()