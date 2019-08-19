cmake_minimum_required(VERSION 3.13)

add_library(ACore STATIC "")

target_sources(ACore PUBLIC  AAddress.h)
target_sources(ACore PRIVATE AAddress.cpp)
target_sources(ACore PUBLIC  ACore.h)
target_sources(ACore PRIVATE ACore.cpp)
target_sources(ACore PUBLIC  ADevice.h)
target_sources(ACore PRIVATE ADevice.cpp)
target_sources(ACore PUBLIC  ADataPoint.h)
target_sources(ACore PRIVATE ADataPoint.cpp)
target_sources(ACore PUBLIC  ATelegram.h)
target_sources(ACore PRIVATE ATelegram.cpp)
target_sources(ACore PRIVATE ATelegramPacket.h)
target_sources(ACore PRIVATE ATelegramPacket.cpp)

target_link_libraries(ACore libhidapi)

get_property(Sources TARGET ACore PROPERTY SOURCES)
source_group("" FILES ${Sources})