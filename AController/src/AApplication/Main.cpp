#include "ACore/ACore.h"
#include "ACore/ADeviceSwitch.h"
#include "ACore/ADeviceRelay.h"

#include <string.h>
#include <future>
#include <iostream>
#include <chrono>

std::string GetLineFromCin() 
{
	std::string line;
	std::getline(std::cin, line);
	return line;
}

int main(int argc, const char** argv)
{
	ACore core;
	core.SetAddress(AIndividualAddress(2, 1, 1));
	core.SetPrintHIDPackets(false);
	core.SetPrintMessages(true);

	ADeviceRelay* relay = new ADeviceRelay();
	relay->SetOnURL("http://192.168.44.51/on");
	relay->SetOffURL("http://192.168.44.51/off");
	relay->SetName("Çalışma Odası-Abajur");
	relay->SetOnOffAddress(AGroupAddress(6,0,4));
	relay->SetOnOffStatusAddress(AGroupAddress(6,0,5));
	core.AddDevice(relay);

	/*ADeviceSwitch* salonYemekAvize = new ADeviceSwitch();
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

	ADeviceSwitch* calismaOdasiAvize = new ADeviceSwitch();
	calismaOdasiAvize->SetName("Calısma-Odasi-Avize");
	calismaOdasiAvize->SetOnOffAddress(AGroupAddress(6, 0, 0));
	calismaOdasiAvize->SetOnOffStatusAddress(AGroupAddress(6, 0, 1));
	core.AddDevice(calismaOdasiAvize);

	core.SetPreLoopCallback(
		[calismaOdasiAvize](ACore* core)
		{
			static auto future = std::async(std::launch::async, GetLineFromCin);
			if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) 
			{
				auto line = future.get();
				future = std::async(std::launch::async, GetLineFromCin);
				if (line == "off")
					calismaOdasiAvize->SetOnOff(false);
				else if (line == "on")
					calismaOdasiAvize->SetOnOff(true);

				std::cout << "you wrote " << line << std::endl;
			}

		}
	);*/

	core.Initialize();
	core.Execute();

	return EXIT_SUCCESS;
}