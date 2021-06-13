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
	bool					m_confirmFlag : 1;
	bool					m_acknowledgeRequestFlag : 1;
	uint8					m_priority : 2;
	uint8					m_systemBroadcast : 1;
	bool					m_repeatFlag : 1;
	uint8					m_reserved : 1;
	uint8					m_frameFormat : 1;

	// Control 2
	uint8					m_extendedFrameFormat : 4;
	uint8					m_hopCount : 3;
	uint8					m_destinationAddresType : 1;
};

struct ACEMIPacketData
{
	ACEMIDataPacketControl	m_control;
	uint16					m_source;
	uint16					m_destination;
	uint8					m_length;
	uint8					m_tapci0;
	uint8					m_tapci1;
};

struct ACEMIPacketProperty
{
	uint16					m_interfaceObjectType;
	uint8					m_objectInstance;
	uint8					m_propertyId;
	uint8					m_arraySize : 4;
	uint8					m_arrayIndexH : 4;
	uint8					m_arrayIndexL;
};

#pragma pack(pop)
