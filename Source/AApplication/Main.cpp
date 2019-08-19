#include "ACore/AAddress.h"
#include "ACore/ATelegram.h"
#include "ACore/ADataPoint.h"

#include <string.h>

int main(int argc, const char** argv)
{
	AAddress address;
	address.SetString("10/11/205");

	
	ADataPoint* Datapoint = new ADataPoint;

	ATelegram Test;
	const char* payload = "orcun here";
	Test.SetCommand(ATelegramCommand::ValueResponse);
	Test.SetDestination(AAddress("1/2/3"));
	Test.SetSource(AAddress("5/6/7"));
	Test.SetPayload(payload, strlen(payload) + 1);
	Test.SetAddressType(ATelegramAddressType::IndividualAddress);
	Test.SetFirstPayload(0x6);
	Test.SetPriority(ATelegramPriority::High);
	Test.SetRepeatFlag(true);

	uint8 buffer[255];
	size_t bufferSize;
	Test.Generate(buffer, bufferSize);

	ATelegram Test1;
	bool result = Test1.Process(buffer, bufferSize);

	return EXIT_SUCCESS;
}