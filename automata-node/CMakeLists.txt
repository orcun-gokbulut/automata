cmake_minimum_required(VERSION 2.8)

set (SOURCE_FILES
    "src/ACore.cpp"
    "src/index.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

message(${CMAKE_JS_INC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ACommon ACore ADevice libhid)
