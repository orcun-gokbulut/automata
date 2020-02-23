#pragma once

#include <vector>

class ADevice;
class ACEMIMessageData;
class AHIDReport;

struct hid_device_;

class ACore
{
	private:
		std::vector<ADevice*>				devices;
		hid_device_*						interfaceDevice;
		size_t								telegramIndex;
		size_t								HIDPacketIndex;

		bool								printHIDPackets;
		bool								printTelegrams;

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

		void								SetPrintTelegrams(bool enabled);
		bool								GetPrintTelegrams() const;

		void								SetPrintHIDPackets(bool enabled);
		bool								GetPrintHIDPackets() const;

		bool								IsInitialized();
		bool								Initialize();
		bool								Deinitialize();

		void								DispatchHIDPacket(const AHIDReport& packet);
		void								DispatchTelegram(const ACEMIMessageData& telegram);

		bool								SendPacket(const AHIDReport& packet);
		bool								SendTelegram(const ACEMIMessageData& telegram);

		void								Process();
		void								Execute();

											ACore();
											~ACore();
};