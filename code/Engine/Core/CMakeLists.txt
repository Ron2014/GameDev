#-------------------------------------------------------------------------------
#   core module
#-------------------------------------------------------------------------------
fips_begin_module(Core)
    fips_vs_warning_level(3)

    fips_include_directories(./)

    if (FIPS_MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    fips_dir(IO)
    fips_files(
		Path.cc Path.h
    )

    fips_dir(Log)
    fips_files(
        Log.cc Log.h
        Logger.cc Logger.h
        StackTrace.cc StackTrace.h
    )

    fips_dir(Math)
    fips_files(
        MathDefs.h
    )

    fips_dir(3D)
    fips_files(
        Vector3D.h
        Vector3D.cc
    )

    fips_dir(Memory)
    fips_files(
        Memory.cc Memory.h
    )

    if(FIPS_WINDOWS)
        fips_dir(private/windows)
        fips_files(
            precompiled.h
            StackWalker.cc StackWalker.h
        )
    else()
        fips_dir(private/posix)
        fips_files(
            precompiled.h
        )
    endif()

    fips_dir(String)
    fips_files(
        StringHelper.cc StringHelper.h
    )

    fips_dir(Object)
    fips_files(
        ObjectManager.h
    )

    fips_dir(Thread)
    fips_files(
        ThreadLocalData.h ThreadLocalData.cc
        ThreadLocalPtr.h
    )

    fips_dir(Util)
    fips_files(
        MemoryBlock.h
        ObjectAllocator.h
        ObjectContainer.h
    )

    if(FIPS_POSIX)
        fips_dir(Util)
        fips_files(
            NcursesUtil.h
        )
    endif()

    fips_dir(.)
    fips_files(
        StdFuture.h
        Assertion.h
        Class.h
        Config.h
        Ptr.h
        RefCounted.h
        SimpleProfiler.h
        Singleton.h
        Core.h Core.cc
        RunLoop.h RunLoop.cc
        Synchronizator.h Synchronizator.cc
    )
fips_end_module()