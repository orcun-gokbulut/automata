#include "ACore/ACore.h"

#include <string.h>

int main(int argc, const char** argv)
{
	ACore core;
	core.Initialize();
	core.SetPrintHIDPackets(false);
	core.SetPrintTelegrams(true);
	core.Execute();

	return EXIT_SUCCESS;
}