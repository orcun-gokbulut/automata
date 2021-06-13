cmake_minimum_required(VERSION 3.10)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

include_directories(${CMAKE_SOURCE_DIR}/automata-lib/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run")

add_subdirectory(src)

install(FILES share/automata.service DESTINATION /lib/systemd/system)
