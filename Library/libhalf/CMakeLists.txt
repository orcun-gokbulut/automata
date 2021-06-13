cmake_minimum_required(VERSION 3.10)

add_library(libhalf INTERFACE)
target_sources(libhalf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/half.hpp)

set_property(TARGET libhalf PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
get_property(Sources TARGET libhalf PROPERTY INTERFACE_SOURCES)
source_group("" FILES ${Sources})
