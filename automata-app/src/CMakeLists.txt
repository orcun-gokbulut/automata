cmake_minimum_required(VERSION 3.10)

set(Sources Main.cpp)

add_executable(automata-app ${Sources})
source_group("" FILES ${Sources})
target_link_libraries(automata-app ACore ADevices ACommon libhidapi udev pthread)
