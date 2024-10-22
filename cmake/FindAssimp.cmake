# FindASSIMP
#
# Locates ASSIMP and sets the following variables:
#
# ASSIMP_FOUND ASSIMP_INCLUDE_DIRS ASSIMP_LIBARY_DIRS ASSIMP_LIBRARIES
# ASSIMP_VERSION_STRING
#
# ASSIMP_FOUND is set to YES only when all other variables are successfully
# configured.

unset(ASSIMP_FOUND)
unset(ASSIMP_INCLUDE_DIRS)
unset(ASSIMP_LIBRARY_DIRS)
unset(ASSIMP_LIBRARIES)

mark_as_advanced(ASSIMP_FOUND)
mark_as_advanced(ASSIMP_INCLUDE_DIRS)
mark_as_advanced(ASSIMP_LIBRARY_DIRS)
mark_as_advanced(ASSIMP_LIBRARIES)

set(assimp_header_file "assimp/mesh.h")

# Set ASSIMP_INCLUDE_DIRS
find_path(ASSIMP_INCLUDE_DIRS NAMES "${assimp_header_file}")
find_library(ASSIMP_LIBRARIES NAMES libassimp.a libassimp.so assimp-vc140-mt.lib assimp-vc140-mt.dll assimp.dll assimp.lib)
get_filename_component(ASSIMP_LIBRARY_DIRS ${ASSIMP_LIBRARIES} DIRECTORY)

if(ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES AND ASSIMP_LIBRARY_DIRS)
  set(ASSIMP_FOUND YES)
  set(ASSIMP_LIBRARIE ${ASSIMP_LIBRARIES})
endif()

if(ASSIMP_FOUND)
    if(NOT ASSIMP_FIND_QUIETLY)
    message(STATUS "Found ASSIMP :${ASSIMP_LIBRARIES}")
    endif(NOT ASSIMP_FIND_QUIETLY)
else(ASSIMP_FOUND)  
    if(NOT ASSIMP_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find ASSIMP")
    endif(NOT ASSIMP_FIND_REQUIRED)
endif(ASSIMP_FOUND)

if(FIND_LOG)
    message(STATUS ${ASSIMP_FOUND})
    message(STATUS ${ASSIMP_INCLUDE_DIRS})
    message(STATUS ${ASSIMP_LIBRARY_DIRS})
    message(STATUS ${ASSIMP_LIBRARIES})
endif()

