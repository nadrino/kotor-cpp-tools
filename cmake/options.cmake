# CMake options

message("")
cmessage( WARNING "Setting up options...")

# dev options
option( USE_STATIC_LINKS "Use static link of libraries and apps instead of shared." OFF )
option( CXX_WARNINGS "Enable most C++ warning flags." ON )
option( CXX_MARCH_FLAG "Enable cpu architecture specific optimisations." OFF )
option( CMAKE_CXX_EXTENSIONS "Enable GNU extensions to C++ language (-std=gnu++14)." OFF )


# Reading options
##################

# Set the default built type if it isn't already defined
if( NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  cmessage( STATUS "Using default build type: Release." )
  set( CMAKE_BUILD_TYPE Release )
else()
  cmessage( STATUS "Using build type: ${CMAKE_BUILD_TYPE}" )
endif()
