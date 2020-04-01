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
		bool								busStatus;
		bool								printHIDPackets;
		bool								printMessages;

		bool								initialized;

		ACoreInitializedCallback			initializationCallback;
		ACoreDeinitializedCallback			deinitializationCallback;
		ACorePreLoopCallback				preLoopCallback;
		ACorePostLoopCallback				postLoopCallback;

		bool								InitializeDevices();
		bool								DeinitializeDevices();

		bool								InitializeInterface();
		bool								DeinitializeInterface();

		void								SendServiceCommand(uint8 command, uint8 feature, const void* parameter, size_t parameterSize);
		void								SendServiceQuery(uint8 command, uint8 feature, const void* parameter, size_t parameterSize, void* output, size_t outputSize, uint32 timeout = 1000);

	public:
		const std::vector<ADevice*>&		GetDevices() const;
		ADevice*							GetDevice(const char* Name) const;
		void								AddDevice(ADevice* device);
		void								RemoveDevice(ADevice* device);

		void								SetAddress(const AIndividualAddress& address);
		const AIndividualAddress&			GetAddress() const;

		void								SetPrintMessages(bool enabled);
		bool								GetPrintMessages() const;

		void								SetPrintHIDPackets(bool enabled);
		bool								GetPrintHIDPackets() const;

		void								QueryBusStatus();
		bool								GetBusStatus();

		bool								IsInitialized();
		bool								Initialize();
		bool								Deinitialize();

		void								DispatchHIDReport(const AHIDReport& report);
		void								DispatchMessage(const ACEMIMessage& message);

		bool								SendHIDReport(const AHIDReport& report);
		bool								ReceiveHIDReport(AHIDReport& report, uint32 timeout = 100);

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