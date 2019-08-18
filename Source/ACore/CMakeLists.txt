cmake_minimum_required(VERSION 3.1)

set(Sources
	AAddress.h
	AAddress.cpp
	ADevice.h
	ADevice.cpp
	ADataPoint.h
	ADataPoint.cpp
	ATelegram.h
	ATelegram.cpp
	ATelegramPacket.h
	ATelegramPacket.cpp)

add_library(ACore ${Sources})
source_group("" FILES ${Sources})