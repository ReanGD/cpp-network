set(PROJECT_CLIENT client)
project(${PROJECT_CLIENT} CXX)

file(GLOB SRC_MAIN
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_CLIENT} ${SRC_MAIN})
target_link_libraries(${PROJECT_CLIENT} ${Boost_LIBRARIES})
