cmake_minimum_required(VERSION 3.1)

set(Sources
	Address.h
	Address.cpp
	Device.h
	Device.cpp
	DataPoint.h
	DataPoint.cpp
	Telegram.h
	Telegram.cpp
	TelegramPacket.h
	TelegramPacket.cpp)

add_library(Core ${Sources})
source_group("" FILES ${Sources})