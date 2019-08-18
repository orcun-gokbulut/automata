cmake_minimum_required(VERSION 3.1)

set(Sources
	ATypes.h
	ATypes.cpp
	AError.h
	AError.cpp)

add_library(ACommon ${Sources})
source_group("" FILES ${Sources})