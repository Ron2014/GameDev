if (FIPS_WINDOWS)
fips_add_subdirectory(PDCurses)
else()
# # if you want to use lib directly in linux
# # no need to add this directory
fips_add_subdirectory(ncurses-6.2)
endif()