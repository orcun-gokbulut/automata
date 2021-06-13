#pragma once

#include"ACommon/ATypes.h"

#pragma pack(push)
#pragma pack(1)

struct AHIDReportHeader
{
	uint8					m_reportID;
	bool					m_startPacket : 1;
	bool					m_endPacket : 1;
	bool					m_partialPacket : 1;
	bool					m_reserved : 1;
	uint8					m_sequenceNumber : 4;
	uint8					m_dataLength;
};

struct AHIDReportBody
{
	uint8					m_protocolVersion;
	uint8					m_headerLength;
	uint16					m_bodyLenght;
	uint8					m_protocolId;
	uint8					m_EMIId;
	uint16					m_manufacturerCode;
};

#pragma pack(pop)
