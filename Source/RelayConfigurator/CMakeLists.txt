cmake_minimum_required(VERSION 3.10)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

list(APPEND Headers ARelayConfiguratorDialog.h)
list(APPEND Sources ARelayConfiguratorDialog.cpp)
list(APPEND UIs     ARelayConfiguratorDialog.ui)
list(APPEND Sources ARelayConfigurator.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(automata-configurator ${Headers} ${Sources} ${UIs})
target_link_libraries(automata-configurator Qt5::Widgets Qt5::Network)

get_property(Sources TARGET automata-configurator PROPERTY SOURCES)
source_group("" FILES ${Sources} ${Headers} ${UIs})

install(TARGETS automata-configurator RUNTIME DESTINATION bin)
