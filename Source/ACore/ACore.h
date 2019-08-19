#pragma once

#include <vector>

class ADevice;
class ACore
{
	private:
		std::vector<ADevice*>				devices;

	public:
		const std::vector<ADevice*>&		GetDevices() const;
		ADevice*							GetDevice(const char* Name) const;
		void								AddDevice(ADevice* device);
		void								RemoveDevice(ADevice* device);

		void								SendTelegram(const ATelegram& telegram);

		virtual void						Process();

		void								Initialize();
		void								Deinitialize();

											ACore();
		virtual								~ACore();
};