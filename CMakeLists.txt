 
cmake_minimum_required(VERSION 3.10)

project(automata C CXX)

link_directories("automata-lib/output/lib")
include_directories("automata-lib/output/include")

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/run")

add_subdirectory(automata-lib)
add_subdirectory(automata-app)
add_subdirectory(automata-configurator)

install(FILES tools/automata.service DESTINATION /lib/systemd/system)
