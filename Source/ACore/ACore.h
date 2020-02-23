#pragma once

#include "AAddress.h"

#include <vector>
#include <functional>

class ADevice;
class ACEMIMessage;
class AHIDReport;

struct hid_device_;

#define ACoreInitializedCallback std::function<void (ACore* core)> 
#define ACoreDeinitializedCallback std::function<void (ACore* core)> 
#define ACorePreLoopCallback std::function<void (ACore* core)> 
#define ACorePostLoopCallback std::function<void (ACore* core)> 

class ACore
{
	private:
		AIndividualAddress					address;
		std::vector<ADevice*>				devices;
		hid_device_*						interfaceDevice;
		size_t								telegramIndex;
		size_t								HIDPacketIndex;

		bool								printHIDPackets;
		bool								printTelegrams;

		bool								initialized;

		ACoreInitializedCallback			initializationCallback;
		ACoreDeinitializedCallback			deinitializationCallback;
		ACorePreLoopCallback				preLoopCallback;
		ACorePostLoopCallback				postLoopCallback;

		bool								InitializeDevices();
		bool								DeinitializeDevices();

		bool								InitializeInterface();
		bool								DeinitializeInterface();

	public:
		const std::vector<ADevice*>&		GetDevices() const;
		ADevice*							GetDevice(const char* Name) const;
		void								AddDevice(ADevice* device);
		void								RemoveDevice(ADevice* device);

		void								SetAddress(const AIndividualAddress& address);
		const AIndividualAddress&			GetAddress() const;

		void								SetPrintTelegrams(bool enabled);
		bool								GetPrintTelegrams() const;

		void								SetPrintHIDPackets(bool enabled);
		bool								GetPrintHIDPackets() const;

		bool								IsInitialized();
		bool								Initialize();
		bool								Deinitialize();

		void								DispatchHIDPacket(const AHIDReport& packet);
		void								DispatchMessage(const ACEMIMessage& message);

		bool								SendPacket(const AHIDReport& packet);
		bool								SendMessage(const ACEMIMessage& message);

		void								Process();
		void								Execute();

		void								SetInitializationCallback(const ACoreInitializedCallback& callback);
		const ACoreInitializedCallback&		GetInitializationCallback() const;

		void								SetDeinitializationCallback(const ACoreDeinitializedCallback& callback);
		const ACoreDeinitializedCallback&	GetDeinitializationCallback() const;

		void								SetPreLoopCallback(const ACorePreLoopCallback& callback);
		const ACorePreLoopCallback&			GetPreLoopCallback() const;

		void								SetPostLoopCallback(const ACorePostLoopCallback& callback);
		const ACorePostLoopCallback&		GetPostLoopCallback() const;

											ACore();
											~ACore();
};