cmake_minimum_required(VERSION 3.13)

project(Automation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

include_directories(${CMAKE_SOURCE_DIR}/Source)

add_subdirectory(Library)
add_subdirectory(Source)

