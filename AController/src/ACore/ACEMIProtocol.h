#pragma once

#include"ACommon/ATypes.h"

#define A_TELEGRAM_MAX_PAYLOAD_SIZE 14
#define A_TELEGRAM_MIN_SIZE 9
#define A_TELEGRAM_PAYLOAD_OFFSET 8

#pragma pack(push)
#pragma pack(1)

struct ACEMIHeader
{
	uint8 messageCode;
};

struct ACEMIPacketHeaderGeneric
{
	uint8 additionalInfoSize;
};

struct ACEMIDataPacketControl
{
	// Control 1
	bool					confirmFlag : 1;
	bool					acknowledgeRequestFlag : 1;
	uint8					priority : 2;
	uint8					systemBroadcast : 1;
	bool					repeatFlag : 1;
	uint8					reserved : 1;
	uint8					frameFormat : 1;
	
	// Control 2
	uint8					extendedFrameFormat : 4;
	uint8					hopCount : 3;
	uint8					destinationAddresType : 1;
};

struct ACEMIPacketData
{
	ACEMIDataPacketControl	control;
	uint16					source;
	uint16					destination;
	uint8					length;
	uint8					tapci0;
	uint8					tapci1;
};

struct ACEMIPacketProperty
{
	uint16					interfaceObjectType;
	uint8					objectInstance;
	uint8					propertyId;
	uint8					arraySize : 4;
	uint8					arrayIndexH : 4;
	uint8					arrayIndexL;
};

#pragma pack(pop)