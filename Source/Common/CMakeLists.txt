cmake_minimum_required(VERSION 3.1)

set(Sources
	Types.h
	Types.cpp
	Error.h
	Error.cpp)

add_library(Common ${Sources})
source_group("" FILES ${Sources})