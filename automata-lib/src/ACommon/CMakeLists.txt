cmake_minimum_required(VERSION 3.10)

include (TestBigEndian)


TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
	target_compile_definitions(ACommon PUBLIC _BIG_ENDIAN)
endif()

list(APPEND Headers ATypes.h)
list(APPEND Sources ATypes.cpp)
list(APPEND Headers AEndian.h)
list(APPEND Sources AEndian.cpp)
list(APPEND Headers AError.h)
list(APPEND Sources AError.cpp)

add_library(ACommon STATIC ${Headers} ${Sources})
target_link_libraries(ACommon libhalf)
source_group("" FILES ${Headers} ${Sources})

install(TARGETS ACommon ARCHIVE DESTINATION lib)
get_target_property(HEADERS ACore INTERFACE_SOURCES)
install(FILES ${Headers} DESTINATION include/ACommon)
