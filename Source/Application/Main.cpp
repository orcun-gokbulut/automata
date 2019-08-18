#include "Core/Address.h"
#include "Core/Telegram.h"

#include <string.h>

int main(int argc, const char** argv)
{
	OHAddress address;
	address.SetString("10/11/205");

	OHTelegram Test;
	const char* payload = "orcun here";
	Test.SetCommand(OHTelegramCommand::ValueResponse);
	Test.SetDestination(OHAddress("1/2/3"));
	Test.SetSource(OHAddress("5/6/7"));
	Test.SetPayload(payload, strlen(payload) + 1);
	Test.SetAddressType(OHTelegramAddressType::IndividualAddress);
	Test.SetFirstPayload(0x6);
	Test.SetPriority(OHTelegramPriority::High);
	Test.SetRepeatFlag(true);

	uint8 buffer[255];
	size_t bufferSize;
	Test.Generate(buffer, bufferSize);

	OHTelegram Test1;
	bool result = Test1.Process(buffer, bufferSize);

	return EXIT_SUCCESS;
}