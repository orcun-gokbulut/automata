cmake_minimum_required(VERSION 3.10)

list(APPEND Sources Main.cpp)

add_executable(automata-app ${Sources})
source_group("" FILES ${Sources})
target_link_libraries(automata-app ACore ADevices ACommon libhidapi udev pthread)

install(TARGETS automata-app RUNTIME DESTINATION bin)
