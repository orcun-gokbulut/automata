cmake_minimum_required(VERSION 3.10)

add_library(liburlparser STATIC "")

target_sources(liburlparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/url_parser.h ${CMAKE_CURRENT_SOURCE_DIR}/url_parser.c)

set_property(TARGET liburlparser  PROPERTY FOLDER "Library")
set_property(TARGET liburlparser  PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

get_property(Sources TARGET liburlparser  PROPERTY SOURCES)
source_group("" FILES ${Sources})

install(TARGETS liburlparser  ARCHIVE DESTINATION lib)
