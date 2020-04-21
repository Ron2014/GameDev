fips_begin_module(Terrain)
    fips_include_directories(./)

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
    endif()
    
    fips_files_ex(. *.h GROUP "Header Files")
    fips_files_ex(. *.cc GROUP "Source Files")

    if (FIPS_WINDOWS)
        fips_deps(Core PDCurses)
    else()
        fips_deps(Core ncurses)
    endif()

fips_end_module()

fips_begin_unittest(Terrain)
    fips_vs_warning_level(3)
    fips_vs_disable_warnings(4819 4996)  # 为了中文注释 cp936
    
    fips_dir(UnitTests)
    if(FIPS_WINDOWS)
    endif()
    fips_files(
        TerrainLoader.cc
    )
    fips_deps(Terrain)
fips_end_unittest()