cmake_minimum_required(VERSION 3.16)

add_library(libhttp INTERFACE)

#set_property(TARGET libhttp PROPERTY FOLDER "Library")
set_property(TARGET libhttp PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(libhttp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/httplib.h)

get_property(Sources TARGET libhttp PROPERTY INTERFACE_SOURCES)
source_group("" FILES ${Sources})