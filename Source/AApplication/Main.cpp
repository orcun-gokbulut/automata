#include "ACore/ACore.h"
#include "ACore/ADeviceSwitch.h"

#include <string.h>

int main(int argc, const char** argv)
{
	ACore core;
	core.SetAddress(AIndividualAddress(2, 1, 1));
	core.SetPrintHIDPackets(true);
	core.SetPrintTelegrams(true);

	ADeviceSwitch* salonYemekAvize = new ADeviceSwitch();
	salonYemekAvize->SetName("Salon-YemekAlani-Avize");
	salonYemekAvize->SetOnOffAddress(AGroupAddress(5, 1, 0));
	salonYemekAvize->SetOnOffStatusAddress(AGroupAddress(5, 1, 1));
	salonYemekAvize->SetDimLevelAddress(AGroupAddress(5, 1, 3));
	salonYemekAvize->SetDimLevelStatusAddress(AGroupAddress(5, 1, 4));
	core.AddDevice(salonYemekAvize);

	ADeviceSwitch* salonOturmaAvize = new ADeviceSwitch();
	salonOturmaAvize->SetName("Salon-Oturma-Avize");
	salonOturmaAvize->SetOnOffAddress(AGroupAddress(5, 1, 5));
	salonOturmaAvize->SetOnOffStatusAddress(AGroupAddress(5, 1, 6));
	salonOturmaAvize->SetDimLevelAddress(AGroupAddress(5, 1, 8));
	salonOturmaAvize->SetDimLevelStatusAddress(AGroupAddress(5, 1, 9));
	core.AddDevice(salonOturmaAvize);

	core.SetInitializationCallback(
		[salonOturmaAvize](ACore* core) 
		{
			salonOturmaAvize->SetDim(0.5);
			salonOturmaAvize->SetOnOff(true);
		}
	);

	core.Initialize();
	core.Execute();

	return EXIT_SUCCESS;
}