cmake_minimum_required(VERSION 3.10)

list(APPEND Headers ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)

add_library(libjson INTERFACE)
target_sources(libjson INTERFACE ${Headers})

set_property(TARGET libjson PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
get_property(Sources TARGET libjson PROPERTY INTERFACE_SOURCES)
source_group("" FILES ${Sources})
