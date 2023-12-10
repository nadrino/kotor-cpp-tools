#
# kotor-cpp-tools project CMake main file
#

# cmake_minimum_required() should be called prior to this top-level project()
# 3.5 minimum a priori. Taking a lower version as min will make recent CMake
# version complain about the deprecation version older than 3.5.
# Might require higher version for specific features.
cmake_minimum_required( VERSION 3.5 FATAL_ERROR )

# Define project
project( kotor-cpp-tools )

# colored messages
include( ${CMAKE_SOURCE_DIR}/cmake/utils/cmessage.cmake )

# define cmake options
include( ${CMAKE_SOURCE_DIR}/cmake/options.cmake )

# git version checking
include( ${CMAKE_SOURCE_DIR}/cmake/version.cmake )

# checking dependencies
include( ${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake )

# setup compiler options
include( ${CMAKE_SOURCE_DIR}/cmake/compiler.cmake )

# setting up submodule libraries
include( ${CMAKE_SOURCE_DIR}/cmake/submodules.cmake )
