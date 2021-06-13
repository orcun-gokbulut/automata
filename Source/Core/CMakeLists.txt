cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(ACore)
add_subdirectory(ACommon)
add_subdirectory(ADevices)

add_library(Automata-Library INTERFACE)
target_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Automata-Library ACore ACommon ADevice)
