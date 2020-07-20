cmake_minimum_required(VERSION 3.10)

add_library(libjson INTERFACE)

#set_property(TARGET libjson PROPERTY FOLDER "Library")
set_property(TARGET libjson PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(libjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)

get_property(Sources TARGET libjson PROPERTY INTERFACE_SOURCES)
source_group("" FILES ${Sources})