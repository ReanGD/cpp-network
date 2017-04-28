set(PROJECT_CLIENT client)
project(${PROJECT_CLIENT} CXX)

aux_source_directory(${PROJECT_SOURCE_DIR} SRC_MAIN)
add_executable(${PROJECT_CLIENT} ${SRC_MAIN})
target_link_libraries(${PROJECT_CLIENT} ${Boost_LIBRARIES})
