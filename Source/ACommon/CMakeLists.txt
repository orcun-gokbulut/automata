cmake_minimum_required(VERSION 3.13)

add_library(ACommon STATIC "")

target_sources(ACommon PUBLIC  ATypes.h)
target_sources(ACommon PRIVATE ATypes.cpp)
target_sources(ACommon PUBLIC  AError.h)
target_sources(ACommon PRIVATE AError.cpp)

get_property(Sources TARGET ACommon PROPERTY SOURCES)
source_group("" FILES ${Sources})