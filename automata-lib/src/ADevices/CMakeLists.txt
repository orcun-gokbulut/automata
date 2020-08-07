cmake_minimum_required(VERSION 3.10)


list(APPEND Headers ADeviceSwitch.h)
list(APPEND Sources ADeviceSwitch.cpp)
list(APPEND Headers ADeviceRelay.h)
list(APPEND Sources ADeviceRelay.cpp)

add_library(ADevices STATIC ${Headers} ${Sources})
target_link_libraries(ADevices libhttp liburlparser)

get_property(Sources TARGET ADevices PROPERTY SOURCES)
source_group("" FILES ${Sources})

install(TARGETS ADevices ARCHIVE DESTINATION lib)
get_target_property(HEADERS ACore INTERFACE_SOURCES)
install(FILES ${Headers} DESTINATION include/ADevices)
