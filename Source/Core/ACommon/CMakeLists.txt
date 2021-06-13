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
source_group("" FILES ${Headers} ${Sources})
target_link_libraries(ACommon libhalf)

install(TARGETS ACommon ARCHIVE DESTINATION lib)
install(FILES ${Headers} DESTINATION include/ACommon)
