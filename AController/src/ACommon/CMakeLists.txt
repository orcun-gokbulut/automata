cmake_minimum_required(VERSION 3.16)

include (TestBigEndian)

add_library(ACommon STATIC "")

TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
	target_compile_definitions(ACommon PUBLIC _BIG_ENDIAN)
endif()

target_sources(ACommon PUBLIC  ATypes.h)
target_sources(ACommon PRIVATE ATypes.cpp)
target_sources(ACommon PUBLIC  AEndian.h)
target_sources(ACommon PRIVATE AEndian.cpp)
target_sources(ACommon PUBLIC  AError.h)
target_sources(ACommon PRIVATE AError.cpp)

get_property(Sources TARGET ACommon PROPERTY SOURCES)
target_link_libraries(ACommon libhalf)
source_group("" FILES ${Sources})