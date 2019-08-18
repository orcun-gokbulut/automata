#include "AAddress.h"

#define A_TELEGRAM_MAX_PAYLOAD_SIZE 14
#define A_TELEGRAM_MIN_SIZE 9
#define A_TELEGRAM_PAYLOAD_OFFSET 8


#pragma pack(push)
#pragma pack(1)

struct ATelegramControlField
{
	uint8					frameFormat : 2;
	bool					repeadFlag : 1;
	bool					reserved0 : 1;
	uint8					priority : 2;
	uint8					reserved1 : 2;
};

struct ATelegramRoutingField
{
	uint8					destinationAddressType : 1;
	uint8					counter : 3;
	uint8					payloadLenght : 4;
};

struct ATelegramCommandField
{
	uint8					Reserved : 2;
	uint8					command : 4;
	uint8					payload : 6;
};

struct ATelegramPacket
{
	ATelegramControlField	control;
	uint16					source;
	uint16					destination;
	ATelegramRoutingField	routing;
	ATelegramCommandField	command;
};

#pragma pack(pop)