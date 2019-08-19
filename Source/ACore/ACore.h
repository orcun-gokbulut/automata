#pragma once

#include <vector>

class ADevice;
class ATelegram;

struct hid_device_;

class ACore
{
	private:
		std::vector<ADevice*>				devices;
		hid_device_*						interfaceDevice;

		bool								initialized;

		bool								InitializeDevices();
		bool								DeinitializeDevices();

		bool								InitializeInterface();
		bool								DeinitializeInterface();

	public:
		const std::vector<ADevice*>&		GetDevices() const;
		ADevice*							GetDevice(const char* Name) const;
		void								AddDevice(ADevice* device);
		void								RemoveDevice(ADevice* device);

		bool								IsInitialized();
		bool								Initialize();
		bool								Deinitialize();

		void								DispatchTelegram(const ATelegram& telegram);
		bool								SendTelegram(const ATelegram& telegram);

		void								Process();
		void								Execute();

											ACore();
											~ACore();
};