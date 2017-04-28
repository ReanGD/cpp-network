set(PROJECT_SERVER server)
project(${PROJECT_SERVER} CXX)

aux_source_directory(${PROJECT_SOURCE_DIR} SRC_MAIN)
add_executable(${PROJECT_SERVER} ${SRC_MAIN})
target_link_libraries(${PROJECT_SERVER} ${Boost_LIBRARIES})
