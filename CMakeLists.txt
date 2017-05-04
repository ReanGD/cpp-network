cmake_minimum_required(VERSION 3.8)


set(PROJECT cpp-network)
project(${PROJECT} CXX)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -pedantic -pthread")


# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system thread log)
if(NOT Boost_FOUND)
    message( FATAL_ERROR "Boost must be installed")
endif()


add_subdirectory(${PROJECT_SOURCE_DIR}/server)
add_subdirectory(${PROJECT_SOURCE_DIR}/client)


