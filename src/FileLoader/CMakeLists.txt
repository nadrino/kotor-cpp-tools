
set( LIB_NAME "KotorFileLoader" )
set( SRC_FILE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LipFile.cpp
)


if( USE_STATIC_LINKS )
  add_library( ${LIB_NAME} STATIC ${SRC_FILE_LIST} )
else()
  add_library( ${LIB_NAME} SHARED ${SRC_FILE_LIST} )
endif()

add_dependencies( ${LIB_NAME} preBuildVersionCheck )
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # version header

# Make sure the current directories are available for the later
# compilation.
target_include_directories( ${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/generated
)

#target_link_libraries( GundamUtils PUBLIC
#)

install(TARGETS ${LIB_NAME} DESTINATION lib)
