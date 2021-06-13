cmake_minimum_required(VERSION 3.10)

add_library(libhidapi STATIC "")
target_sources(libhidapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/hidapi.h)

if (WIN32)
	target_sources(libhidapi PRIVATE hid_win32.c)
	set_property(TARGET libhidapi PROPERTY INTERFACE_LINK_LIBRARIES Setupapi.lib)
elseif(APPLE)
	target_sources(libhidapi PRIVATE hid_macosx.c)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_sources(libhidapi PRIVATE hid_linux.c)
	set_property(TARGET libhidapi PROPERTY INTERFACE_LINK_LIBRARIES udev pthread)
endif()

set_property(TARGET libhidapi PROPERTY FOLDER "Library")
set_property(TARGET libhidapi PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
get_property(Sources TARGET libhidapi PROPERTY SOURCES)
source_group("" FILES ${Sources})

install(TARGETS libhidapi ARCHIVE DESTINATION lib)
