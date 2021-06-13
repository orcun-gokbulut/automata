
cmake_minimum_required(VERSION 3.10)

set(AUTOMATA_BUILD_SERVICE true CACHE BOOL "Build service application.")
set(AUTOMATA_BUILD_NODE_PACKAGE false CACHE BOOL "Build Node.js Package.")
set(AUTOMATA_BUILD_RELAY false CACHE BOOL "Build relay firmware.")
set(AUTOMATA_BUILD_RELAY_CONFIGURATOR true CACHE BOOL "Build relay configurator utility.")

if(AUTOMATA_BUILD_RELAY)
    add_subdirectory(Relay)
else()
    add_subdirectory(Core)

    if (AUTOMATA_BUILD_SERVICE)
        add_subdirectory(Service)
    endif()

    if(AUTOMATA_BUILD_RELAY_CONFIGURATOR)
        add_subdirectory(RelayConfigurator)
    endif()

    if(AUTOMATA_BUILD_RELAY_NODE_PACKAGE)
        add_subdirectory(Node)
    endif()
endif()
