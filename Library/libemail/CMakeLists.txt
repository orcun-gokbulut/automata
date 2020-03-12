cmake_minimum_required(VERSION 3.13)

add_library(libemail STATIC "")

set_property(TARGET libemail PROPERTY FOLDER "Library")
set_property(TARGET libemail PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(libemail PUBLIC email.h)
target_sources(libemail PRIVATE email.cpp)

get_property(Sources TARGET libemail PROPERTY SOURCES)
source_group("" FILES ${Sources})