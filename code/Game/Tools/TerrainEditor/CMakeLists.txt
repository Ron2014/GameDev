fips_begin_app(TerrainEditor cmdline)
    fips_vs_warning_level(3)
    fips_vs_disable_warnings(4005 4996)
    
    fips_include_directories(.)

    fips_dir(.)
    fips_files(
        main.cc
    )

    fips_deps(UICurses)
fips_end_app()