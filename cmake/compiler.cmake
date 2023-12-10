

message("")
cmessage( WARNING "Configuring compiler options...")

# Changes default install path to be a subdirectory of the build dir.
# Should set the installation dir at configure time with
# -DCMAKE_INSTALL_PREFIX=/install/path
if(NOT DEFINED CMAKE_INSTALL_PREFIX
    OR CMAKE_INSTALL_PREFIX STREQUAL ""
    OR CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

cmessage( STATUS "CMAKE_INSTALL_PREFIX: \"${CMAKE_INSTALL_PREFIX}\"")


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.5)
    cmessage(STATUS "Detected GCC version: ${CMAKE_CXX_COMPILER_VERSION}")
    cmessage(FATAL_ERROR "GCC version must be at least 5.0")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" )
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
    cmessage( STATUS "Detected Clang version: ${CMAKE_CXX_COMPILER_VERSION}" )
    cmessage( FATAL_ERROR "Clang version must be at least 3.3" )
  endif()
else()
  cmessage( ALERT "You are using an untested compiler." )
endif()

# CXX standard is required and must match the version ROOT was compiled with.
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 20 )

if(CXX_WARNINGS)
  cmessage( STATUS "Enable CXX warnings" )
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wall>)
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-variable>)
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-sign-compare>)
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-function>)
  # add_compile_options(-Wformat=0)
  # add_compile_options(-Wno-reorder)
endif()

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC -g" )

################################################################################
# CMake Generated
###############################################################################

cmessage( STATUS "C++ Compiler      : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
cmessage( STATUS "C++ Standard      : ${CMAKE_CXX_STANDARD}" )
cmessage( STATUS "C++ Release flags : ${CMAKE_CXX_FLAGS_RELEASE}" )
cmessage( STATUS "C++ Debug flags   : ${CMAKE_CXX_FLAGS_DEBUG}" )
cmessage( STATUS "Build type        : ${CMAKE_BUILD_TYPE}" )

