#
# project: GameDev
#
cmake_minimum_required(VERSION 2.8)
project(GameDev)
# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

fips_setup()

# POSIX platform defines
if (FIPS_POSIX)
    add_definitions(-DFUTURE_POSIX=1)
    if (FIPS_LINUX)
        add_definitions(-DFUTURE_LINUX=1)
    endif()
    if (FIPS_OSX)
        add_definitions(-DFUTURE_OSX=1)
        if (FIPS_MACOS)
            add_definitions(-DFUTURE_MACOS=1)
        endif()
        if (FIPS_IOS)
            add_definitions(-DFUTURE_IOS=1)
        endif()
    endif()
    if (FIPS_ANDROID)
        add_definitions(-DFUTURE_ANDROID=1)
    endif()

    add_definitions(-D_strnicmp=strncasecmp)
endif()

# Windows platform defines
if (FIPS_WINDOWS)
    add_definitions(-DFUTURE_WINDOWS=1)
    if (FIPS_WIN32)
        add_definitions(-DFUTURE_WIN32=1)
    endif()
    if (FIPS_WIN64)
        add_definitions(-DFUTURE_WIN64=1)
    endif()
endif()

fips_include_directories(.)
fips_include_directories(Engine)
fips_include_directories(Ext)

if (FIPS_WINDOWS)
    fips_include_directories(Ext/PDCurses)
else()
    # # if you want to use lib directly in linux
    # # enter ncurses-6.2 do these command
    # # $./configure && sudo make
    # # and then use the library like this

    # fips_include_directories(Ext/ncurses-6.2/include)
    # link_directories(Ext/ncurses-6.2/lib)
    # fips_libs(
    #     libncurses_g.a
    #     libncurses.a
    #     libncurses++_g.a
    #     libncurses++.a
    # )
    fips_include_directories(Ext/ncurses-6.2/include)
    fips_include_directories(Ext/ncurses-6.2/ncurses)
endif()

fips_include_directories(Game)
fips_include_directories(Game/ECS)
fips_include_directories(Game/UICurses)

fips_ide_group(Ext)
fips_add_subdirectory(Ext)

fips_ide_group(Engine)
fips_add_subdirectory(Engine)

fips_ide_group(Game)
fips_add_subdirectory(Game)

fips_finish()


