cmake_minimum_required(VERSION 3.10)


list(APPEND Headers ADeviceSwitch.h)
list(APPEND Sources ADeviceSwitch.cpp)
list(APPEND Headers ADeviceRelay.h)
list(APPEND Sources ADeviceRelay.cpp)

add_library(ADevices STATIC ${Headers} ${Sources})
source_group("" FILES ${Sources})
target_link_libraries(ADevices libhttp)

install(FILES ${Headers} DESTINATION include/ADevices)
install(TARGETS ADevices ARCHIVE DESTINATION lib)
