# FindLASLIB
#
# Locates LASLIB and sets the following variables:
#
# LASLIB_FOUND LASLIB_INCLUDE_DIRS LASLIB_LIBARY_DIRS LASLIB_LIBRARIES
# LASLIB_VERSION_STRING
#
# LASLIB_FOUND is set to YES only when all other variables are successfully
# configured.

unset(LASLIB_FOUND)
unset(LASLIB_INCLUDE_DIRS)
unset(LASLIB_LIBRARY_DIRS)
unset(LASLIB_LIBRARIES)

mark_as_advanced(LASLIB_FOUND)
mark_as_advanced(LASLIB_INCLUDE_DIRS)
mark_as_advanced(LASLIB_LIBRARY_DIRS)
mark_as_advanced(LASLIB_LIBRARIES)

set(laslib_header_file "LasLib/lasutility.hpp")

# Set LASLIB_INCLUDE_DIRS
find_path(LASLIB_INCLUDE_DIRS NAMES "${laslib_header_file}")
find_library(LASLIB_LIBRARIES Laslib)
get_filename_component(LASLIB_LIBRARY_DIRS ${LASLIB_LIBRARIES} DIRECTORY)

if(LASLIB_INCLUDE_DIRS AND LASLIB_LIBRARIES AND LASLIB_LIBRARY_DIRS)
  set(LASLIB_FOUND YES)
  set(LASLIB_LIBRARIE ${LASLIB_LIBRARIES})
endif()

if(LASLIB_FOUND)
    if(NOT LASLIB_FIND_QUIETLY)
    message(STATUS "Found LASLIB :${LASLIB_LIBRARIES}")
    endif(NOT LASLIB_FIND_QUIETLY)
else(LASLIB_FOUND)  
    if(NOT LASLIB_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find LASLIB")
    endif(NOT LASLIB_FIND_REQUIRED)
endif(LASLIB_FOUND)

if(FIND_LOG)
    message(STATUS ${LASLIB_FOUND})
    message(STATUS ${LASLIB_INCLUDE_DIRS})
    message(STATUS ${LASLIB_LIBRARIES})
    message(STATUS ${LASLIB_LIBRARY_DIRS})
endif()