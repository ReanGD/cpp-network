cmake_minimum_required(VERSION 2.8)

set(PROJECT cpp-network)
project(${PROJECT} CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wextra -pedantic -pthread")

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system thread)
if(NOT Boost_FOUND)
    message( FATAL_ERROR "Boost must be installed")
endif()
include_directories(${Boost_INCLUDE_DIRS})

# Server
add_subdirectory(${PROJECT_SOURCE_DIR}/server)

# Client
add_subdirectory(${PROJECT_SOURCE_DIR}/client)


