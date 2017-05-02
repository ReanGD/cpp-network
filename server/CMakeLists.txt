set(PROJECT_SERVER server)
project(${PROJECT_SERVER} CXX)

file(GLOB SRC_MAIN
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_SERVER} ${SRC_MAIN})
target_link_libraries(${PROJECT_SERVER} ${Boost_LIBRARIES})
