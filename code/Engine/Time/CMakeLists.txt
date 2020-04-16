#-------------------------------------------------------------------------------
#   Time module
#-------------------------------------------------------------------------------
fips_begin_module(Time)
    fips_vs_warning_level(3)
    fips_files(
        Clock.cc Clock.h
        Tick.cc Tick.h
    )
    
    fips_dir(Tick)
    fips_files(
        Enums.h 
        tickMgr.cc tickMgr.h 
    )

    fips_deps(Core)
fips_end_module()

#fips_begin_unittest(Time)
#    fips_vs_warning_level(3)
#    fips_dir(UnitTests)
#    fips_files(
#        ClockTest.cc
#        DurationTest.cc
#        TimePointTest.cc
#    )
#    fips_deps(Time Core)
#fips_end_unittest()
