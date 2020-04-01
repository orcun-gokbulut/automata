cmake_minimum_required(VERSION 3.16)

add_library(ACore STATIC "")

target_sources(ACore PUBLIC  AAddress.h)
target_sources(ACore PRIVATE AAddress.cpp)
target_sources(ACore PUBLIC  ACore.h)
target_sources(ACore PRIVATE ACore.cpp)
target_sources(ACore PUBLIC  ADataPoint.h)
target_sources(ACore PRIVATE ADataPoint.cpp)
target_sources(ACore PUBLIC  AHIDReport.h)
target_sources(ACore PRIVATE AHIDReport.cpp)
target_sources(ACore PUBLIC  AHIDProtocol.h)
target_sources(ACore PRIVATE AHIDProtocol.cpp)
target_sources(ACore PUBLIC  ACEMIMessage.h)
target_sources(ACore PRIVATE ACEMIMessage.cpp)
target_sources(ACore PRIVATE ACEMIProtocol.h)
target_sources(ACore PRIVATE ACEMIProtocol.cpp)

target_sources(ACore PUBLIC  ADevice.h)
target_sources(ACore PRIVATE ADevice.cpp)
target_sources(ACore PUBLIC  ADeviceSwitch.h)
target_sources(ACore PRIVATE ADeviceSwitch.cpp)

target_link_libraries(ACore libhidapi libhalf)

get_property(Sources TARGET ACore PROPERTY SOURCES)
source_group("" FILES ${Sources})