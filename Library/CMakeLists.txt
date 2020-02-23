cmake_minimum_required(VERSION 3.1)

add_subdirectory(hidapi)

add_library(libhalf INTERFACE)
target_sources(libhalf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/half.hpp)
get_property(Sources TARGET libhalf PROPERTY INTERFACE_SOURCES)
source_group("" FILES ${Sources})