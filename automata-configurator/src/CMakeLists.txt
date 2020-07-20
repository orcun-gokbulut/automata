cmake_minimum_required(VERSION 3.10)

list(APPEND Headers ARelayConfiguratorDialog.h)
list(APPEND Sources ARelayConfiguratorDialog.cpp)
list(APPEND UIs      ARelayConfiguratorDialog.ui)
list(APPEND Sources ARelayConfigurator.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(ARelayConfigurator ${Headers} ${Sources} ${UIs})
target_link_libraries(ARelayConfigurator Qt5::Widgets Qt5::Network)

get_property(Sources TARGET ARelayConfigurator PROPERTY SOURCES)
source_group("" FILES ${Sources} ${Headers} ${UIs})

#install(TARGETS ADevices LIBRARY DESTINATION lib)
#get_target_property(HEADERS ACore INTERFACE_SOURCES)
#install(FILES ${Headers} DESTINATION include/ADevices)