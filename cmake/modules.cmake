
message("")
cmessage( WARNING "Defining modules...")

set( MODULES FileLoader )

foreach(mod ${MODULES})
  cmessage( STATUS "Adding cmake module: ${mod}" )
  add_subdirectory( ${CMAKE_SOURCE_DIR}/src/${mod} )
endforeach()
