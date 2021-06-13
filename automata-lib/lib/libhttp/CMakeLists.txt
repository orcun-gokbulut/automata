cmake_minimum_required(VERSION 3.10)

add_library(libhttp INTERFACE)
target_sources(libhttp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/httplib.h)

set_property(TARGET libhttp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
get_property(Sources TARGET libhttp PROPERTY INTERFACE_SOURCES)
source_group("" FILES ${Sources})
