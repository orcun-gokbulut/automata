#include "ACore/ACore.h"
#include "ADevices/ADeviceSwitch.h"
#include "ADevices/ADeviceRelay.h"

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
	core.SetPrintMessages(false);

	ADeviceRelay* relay00 = new ADeviceRelay();
	relay00->SetOnURL("http://192.168.44.51/on");
	relay00->SetOffURL("http://192.168.44.51/off");
	relay00->SetName("CalismaOdasi-Abajur-00");
	relay00->SetOnOffAddress(AGroupAddress(6,0,4));
	relay00->SetOnOffStatusAddress(AGroupAddress(6,0,5));
	core.AddDevice(relay00);

    ADeviceRelay* relay01 = new ADeviceRelay();
	relay01->SetOnURL("http://192.168.44.52/on");
	relay01->SetOffURL("http://192.168.44.52/off");
	relay01->SetName("CalismaOdasi-Abajur-01");
	relay01->SetOnOffAddress(AGroupAddress(6,0,6));
	relay01->SetOnOffStatusAddress(AGroupAddress(6,0,7));
	core.AddDevice(relay01);

    ADeviceRelay* relay02 = new ADeviceRelay();
	relay02->SetOnURL("http://192.168.44.53/on");
	relay02->SetOffURL("http://192.168.44.53/off");
	relay02->SetName("Salon-OturmaAlani-Abajur-00");
	relay02->SetOnOffAddress(AGroupAddress(5,0,8));
	relay02->SetOnOffStatusAddress(AGroupAddress(5,0,9));
	core.AddDevice(relay02);

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
