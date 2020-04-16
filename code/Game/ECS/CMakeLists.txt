fips_begin_module(ECS)
    fips_include_directories(.)

    fips_files_ex(Entity *.* GROUP "Entity")
    fips_files_ex(Component *.* GROUP "Component")
    fips_files_ex(System *.* GROUP "System")
    fips_files_ex(World *.* GROUP "World")
    
    fips_dir(.)
    fips_files(
        Global.h
    )

    fips_deps(Terrain)
fips_end_module()