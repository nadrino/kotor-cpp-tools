

message("")
cmessage( WARNING "Checking dependencies...")

####################
# NLOHMANN JSON
####################

cmessage( STATUS "Looking for JSON install..." )
find_package( nlohmann_json QUIET )

if( nlohmann_json_FOUND )
  cmessage( STATUS "nlohmann JSON library found.")
  link_libraries( nlohmann_json::nlohmann_json )
else()
  cmessage( ALERT "nlohmann JSON library not found. Using fetch content... (CMake version >= 3.11)")
  cmake_minimum_required( VERSION 3.11 FATAL_ERROR )
  include( FetchContent )

  FetchContent_Declare( json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz )
  FetchContent_MakeAvailable( json )

  include_directories( ${nlohmann_json_SOURCE_DIR}/include )
  cmessage( STATUS "nlohmann JSON library fetched: ${nlohmann_json_SOURCE_DIR}/include")
endif()
