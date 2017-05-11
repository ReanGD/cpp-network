cmake_minimum_required(VERSION 3.8)

set(PROJECT cpp-network)
project(${PROJECT} CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -pedantic")

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system thread log)
if(NOT Boost_FOUND)
    message( FATAL_ERROR "Boost must be installed")
endif()

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

list(REMOVE_ITEM SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main_server.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main_client.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library("${PROJECT_NAME}" STATIC ${SOURCE_FILES})
set(LIBS "${PROJECT_NAME}" pthread ${Boost_LIBRARIES})

set(PROJECT_SERVER server)
add_executable(${PROJECT_SERVER} "${CMAKE_CURRENT_SOURCE_DIR}/src/main_server.cpp")
target_link_libraries(${PROJECT_SERVER} ${LIBS})

set(PROJECT_CLIENT client)
add_executable(${PROJECT_CLIENT} "${CMAKE_CURRENT_SOURCE_DIR}/src/main_client.cpp")
target_link_libraries(${PROJECT_CLIENT} ${LIBS})
