fips_begin_app(TerrainEditor cmdline)

fips_dir(.)
fips_files(
    main.cc

    EntityPainter.h
    ComponentPainter.h
    SystemPainter.h
    SystemPainter.cc
)

fips_deps(Terrain ECS)
fips_end_app()