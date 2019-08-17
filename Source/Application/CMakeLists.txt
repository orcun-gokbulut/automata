cmake_minimum_required(VERSION 3.1)

set(Sources Main.cpp)

add_executable(Application ${Sources})
source_group("" FILES ${Sources})
target_link_libraries(Application Core Common)