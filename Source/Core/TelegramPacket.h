#include "Address.h"

#define TELEGRAM_MAX_PAYLOAD_SIZE 16
#define TELEGRAM_MIN_SIZE 8
#define TELEGRAM_PAYLOAD_OFFSET 7


#pragma pack(push)
#pragma pack(1)

struct OHTelegramControlField
{
	uint8					frameFormat : 2;
	bool					repeadFlag : 1;
	bool					reserved0 : 1;
	uint8					priority : 2;
	uint8					reserved1 : 2;
};

struct OHTelegramRoutingField
{
	uint8					destinationAddressType : 1;
	uint8					counter : 3;
	uint8					payloadLenght : 4;
};

struct OHTelegramCommandField
{
	uint8					Reserved : 2;
	uint8					command : 4;
	uint8					payload : 6;
};

struct OHTelegramPacket
{
	OHTelegramControlField	control;
	uint16					source;
	uint16					destination;
	OHTelegramRoutingField	routing;
	OHTelegramCommandField	command;
};

#pragma pack(pop)