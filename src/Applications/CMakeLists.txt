

set( APP_SRC_LIST
    kotorLipReader
    kotorMdlReader
    kotorGffReader
)

# Now define the frontends
foreach( app ${APP_SRC_LIST} )
  cmessage( STATUS "Defining app: ${app}" )
  add_executable( ${app} ${CMAKE_CURRENT_SOURCE_DIR}/src/${app}.cxx )
  install( TARGETS ${app} DESTINATION bin )
endforeach()

target_link_libraries( kotorLipReader KotorFileLoader )
target_link_libraries( kotorMdlReader KotorFileLoader )
target_link_libraries( kotorGffReader KotorFileLoader )

