fips_begin_module(Terrain)
    fips_include_directories(./)
    fips_files_ex(. *.h GROUP "Header Files")
    fips_files_ex(. *.cc GROUP "Source Files")
    fips_deps(Core)
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