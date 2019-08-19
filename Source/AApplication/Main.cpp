#include "ACore/ACore.h"
#include "ACore/AAddress.h"
#include "ACore/ATelegram.h"
#include "ACore/ADataPoint.h"

#include <string.h>

int main(int argc, const char** argv)
{
	ACore Core;
	Core.Initialize();

	AAddress address;
	address.SetString("10/11/205");
	ADataPoint* Datapoint = new ADataPoint;

	Core.Execute();

	return EXIT_SUCCESS;
}