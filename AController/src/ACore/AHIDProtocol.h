#pragma once

#include"ACommon/ATypes.h"

#pragma pack(push)
#pragma pack(1)

struct AHIDReportHeader
{
	uint8					reportID;
	bool					startPacket : 1;
	bool					endPacket : 1;
	bool					partialPacket : 1;
	bool					reserved : 1;
	uint8					sequenceNumber : 4;
	uint8					dataLength;
};

struct AHIDReportBody
{
	uint8					protocolVersion;
	uint8					headerLength;
	uint16					bodyLenght;
	uint8					protocolId;
	uint8					EMIId;
	uint16					manufacturerCode;
};

#pragma pack(pop)