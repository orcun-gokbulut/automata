#include "ACore.h"

#include "ADevice.h"
#include "AHIDReport.h"
#include "ACEMIMessage.h"

#include "ACommon/AError.h"
#include "ACommon/AEndian.h"

#include <iostream>
#include <hidapi.h>


using namespace std;

bool ACore::InitializeDevices()
{
	Log("Initializing devices.");
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);

		Log("Intializing Device. Device Name: %s.", currentDevice->GetName());

		if (!currentDevice->Initialize())
			RaiseError("Device initialization failed. Device Name: %s", (*iterator)->GetName());
	}

	return true;
}

bool ACore::DeinitializeDevices()
{
	Log("Deinitializing devices.");
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);

		Log("Deinitializing Device. Device Name: %s.", currentDevice->GetName());

		if (!currentDevice->Initialize())
			RaiseError("Device deinitializing failed. Device Name: %s", (*iterator)->GetName());
	}

	return true;
}

bool ACore::InitializeInterface()
{
	CheckError(interfaceDevice != NULL, false, "Cannot initialize interface. Interface is already intialized.");
	
	if (hid_init())
	{
		RaiseError("Cannot initialize HID library.");
		return false;
	}

	Log("Enumerating HID devices for KNX Interface.");
	hid_device_info* firstDeviceInfo = hid_enumerate(0x0, 0x0);
	hid_device_info* currentDeviceInfo = firstDeviceInfo;

	uint16 productId = 0;
	uint16 vendorId = 0;
	while (currentDeviceInfo != nullptr)
	{
		Log("  Enumerating device. Manufacturer: %ls. Product: %ls. VID: %04hx. PID: %04hx.",
			currentDeviceInfo->manufacturer_string,
			currentDeviceInfo->product_string,
			currentDeviceInfo->vendor_id,
			currentDeviceInfo->product_id);

		if (currentDeviceInfo->vendor_id == 0x28c2 && currentDeviceInfo->product_id == 0x0017)
		{
			Log("  USB KNX Interface device found.\n"
				"    Vendor Id:     %04hx\n"
				"    Product Id:    %04hx\n"
				"    Serial Number: %ls\n"
				"    Manufacturer:  %ls\n"
				"    Product:       %ls\n"
				"    Release:       %hx\n"
				"    Interface:     %d\n"
				"    Path:          %ls",
				currentDeviceInfo->vendor_id,
				currentDeviceInfo->product_id,
				currentDeviceInfo->serial_number,
				currentDeviceInfo->manufacturer_string,
				currentDeviceInfo->product_string,
				currentDeviceInfo->release_number,
				currentDeviceInfo->interface_number,
				currentDeviceInfo->path);

			vendorId = currentDeviceInfo->vendor_id;
			productId = currentDeviceInfo->product_id;
			
			break;
		}

		currentDeviceInfo = currentDeviceInfo->next;
	}
	hid_free_enumeration(firstDeviceInfo);

	CheckError(productId == 0 && vendorId == 0, false, "Cannot initialize interface. No KNX USB Interface device has been found.");


	interfaceDevice = hid_open(vendorId, productId, NULL);
	CheckError(interfaceDevice == nullptr, false, "Cannot initialize interface. Cannot open interface device.");

	//hid_set_nonblocking(interfaceDevice, 1);

	Log("Initializing HID Device...");

	uint16 supportedEMIs = 0;
	SendServiceQuery(0x01, 0x01, nullptr, 0, &supportedEMIs, 2);
	supportedEMIs = bswap_16(supportedEMIs);

	Log("Supported EMIs : %s%s%s",
		(supportedEMIs & 0x0001) ? "EMI1 " : "",
		(supportedEMIs & 0x0002) ? "EMI2 " : "",
		(supportedEMIs & 0x0004) ? "cEMI " : "");

	if ((supportedEMIs & 0x0004) != 0x0004)
	{
		RaiseError("Device does not support cEMI. Cannot initialize interface.");
		hid_close(interfaceDevice);
		return false;
	}
	
	uint8 currentEMI = 0;
	SendServiceQuery(0x01, 0x04, nullptr, 0, &currentEMI, 1);
	Log("Active EMI : %s%s%s%s.",
		currentEMI == 1 ? "EMI1 " : "",
		currentEMI == 2 ? "EMI2 " : "",
		currentEMI == 3 ? "cEMI " : "",
		currentEMI == 0 || currentEMI > 3 ? "UNKNOWN" : "");

	if (currentEMI != 0x03)
	{
		Log("Changing EMI to cEMI.");
		
		currentEMI = 0x03;
		SendServiceCommand(0x03, 0x05, &currentEMI, 1);
	}

	Log("HID Device initialized.");

	return true;
}

bool ACore::DeinitializeInterface()
{
	CheckError(interfaceDevice != NULL, false, "Cannot deinitialize interface. Interface is already deinitialized.");

	hid_close(interfaceDevice);
	interfaceDevice = nullptr;
	hid_exit();

	return true;
}

void ACore::SendServiceCommand(uint8 service, uint8 feature, const void* parameter, size_t parameterSize)
{
	AHIDReport packet;
	packet.SetProtocolId(AHIDProtocolId::BusAccessServerFeatureService);
	packet.SetEMIId(service);
	packet.AddData(&feature, 1);
	packet.AddData(parameter, parameterSize);
	SendHIDReport(packet);
}

void ACore::SendServiceQuery(uint8 service, uint8 feature, const void* parameter, size_t parameterSize, void* output, size_t outputSize, uint32 timeOut)
{
	SendServiceCommand(service, feature, parameter, parameterSize);

	AHIDReport outputReport;
	while (true)
	{
		if (!ReceiveHIDReport(outputReport))
			continue;

		if (outputReport.GetProtocolId() == AHIDProtocolId::BusAccessServerFeatureService &&
			outputReport.GetEMIId() == 0x02 &&
			outputReport.GetDataSize() != 0 &&
			*(uint8*)outputReport.GetData() == feature)
		{
			memcpy(output, (uint8*)outputReport.GetData() + 1, outputSize);
			break;
		}
		else
		{
			DispatchHIDReport(outputReport);
		}
	}
}

const std::vector<ADevice*>& ACore::GetDevices() const
{
	return devices;
}

ADevice* ACore::GetDevice(const char* Name) const
{
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		if (strcmp((*iterator)->GetName(), Name) == 0)
			return *iterator;
	}

	return nullptr;
}

void ACore::AddDevice(ADevice* device)
{
	CheckError(device == NULL, RETURN_VOID, "Cannot add device to core. Device is NULL.");
	CheckError(device->GetCore() != nullptr, RETURN_VOID,"Cannot add device to core. Device is already added to a core. Device Name: %s.", device->GetName());

	devices.insert(devices.end(), device);
	device->core = this;

	if (!device->IsInitialized())
		device->Initialize();
}

void ACore::RemoveDevice(ADevice* device)
{
	CheckError(device == NULL, RETURN_VOID, "Cannot remove device from core. Device is NULL.");
	CheckError(device->GetCore() == this, RETURN_VOID, "Cannot remove device from core. Device is not added to this core. Device Name: %s.", device->GetName());

	if (device->IsInitialized())
		device->Deinitialize();

	devices.erase(std::find(devices.begin(), devices.end(), device));
	device->core = nullptr;
}

void ACore::SetAddress(const AIndividualAddress& address)
{
	this->address = address;
}

const AIndividualAddress& ACore::GetAddress() const
{
	return address;
}

void ACore::SetPrintMessages(bool enabled)
{
	printMessages = enabled;
}

bool ACore::GetPrintMessages() const
{
	return printMessages;
}

void ACore::SetPrintHIDPackets(bool enabled)
{
	printHIDPackets = enabled;
}

bool ACore::GetPrintHIDPackets() const
{
	return printHIDPackets;
}

void ACore::QueryBusStatus()
{
	uint8 command = 0x03;
	bool output;
	SendServiceQuery(0x01, 0x03, nullptr, 0, &output, 1);
}

bool ACore::GetBusStatus()
{
	return busStatus;

}

bool ACore::IsInitialized()
{
	return initialized;
}

bool ACore::Initialize()
{
	Log("Initializing core...");
	
	if (!InitializeInterface())
	{
		RaiseError("Core initialization has failed.");
		return false;
	}

	InitializeDevices();

	initialized = true;

	Log("Core initialized.");

	if (initializationCallback != nullptr)
		initializationCallback(this);

	return true;
}

bool ACore::Deinitialize()
{
	Log("Deinitializing core...");

	if (deinitializationCallback != nullptr)
		deinitializationCallback(this);

	DeinitializeDevices();
	DeinitializeInterface();

	initialized = false;

	Log("Core deinitialized.");

	return true;
}

void ACore::DispatchHIDReport(const AHIDReport& report)
{
	if (!initialized)
		return;
	if (report.GetProtocolId() == AHIDProtocolId::BusAccessServerFeatureService)
	{
		if (report.GetEMIId() == 0x04 && report.GetDataSize() == 2 && *(uint8*)report.GetData() == 0x03)
		{
			busStatus = (bool)((uint8*)report.GetData())[1];
		}
	}
	else if (report.GetProtocolId() == AHIDProtocolId::KNXTunnel)
	{
		if (report.GetDataSize() > 0 && *(uint8*)report.GetData() == 0x29)
		{
			ACEMIMessageData telegram;
			if (!telegram.Process(report.GetData(), report.GetDataSize()))
				return;

			telegramIndex++;
			telegram.SetIndex(telegramIndex);

			DispatchMessage(telegram);
		}
	}
}

void ACore::DispatchMessage(const ACEMIMessage& message)
{
	if (!initialized)
		return;

	if (printMessages)
		cout << "INCOMMING " << message.ToString();
	
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);
		
		if (!currentDevice->IsInitialized())
			continue;

		currentDevice->TelegramReceived(message);
	}
}

bool ACore::SendHIDReport(const AHIDReport& report)
{
	uint8 size;
	uint8 packetBuffer[256];
	report.Generate(packetBuffer, size);
	AHIDReport regenerate;
	regenerate.Process(packetBuffer, size);
	
	if (printHIDPackets)
		cout << "OUTGOING " << report.ToString();

	int bytesSend = hid_write(interfaceDevice, packetBuffer, size);
	CheckError(bytesSend < size, false, "Cannot send packet.");

	return true;
}

bool ACore::ReceiveHIDReport(AHIDReport& report, uint32 timeout)
{
	uint8 buffer[1024];
	uint8* cursor = buffer;
	int bytesReceived = hid_read_timeout(interfaceDevice, buffer, 1024, timeout);
	if (bytesReceived == 0)
		return false;
	
	bool result = report.Process(buffer, bytesReceived);

	if (result && printHIDPackets)
		cout << "INCOMMING " << report.ToString();

	return result;
}

bool ACore::SendMessage(const ACEMIMessage& message)
{
	CheckError(!IsInitialized(), false, "Cannot send telegram. Core is not initialized.");

	uint8 buffer[256];
	uint8 size = 0;
	message.Generate(buffer, size);

	if (printMessages)
		cout << "INCOMMING " << message.ToString();

	AHIDReport report;
	report.SetData(buffer, size);
	return SendHIDReport(report);
}

void ACore::Process()
{
	CheckError(!IsInitialized(), RETURN_VOID, "Cannot process core. Core is not initialized.");

	if (preLoopCallback != nullptr)
		preLoopCallback(this);

	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);
		if (!currentDevice->IsInitialized())
			continue;

		currentDevice->PreProcess();
	}

	AHIDReport packet;
	if (ReceiveHIDReport(packet))
	{
		HIDPacketIndex++;
		packet.SetIndex(HIDPacketIndex);
		DispatchHIDReport(packet);
	}

	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);
		if (!currentDevice->IsInitialized())
			continue;

		currentDevice->PostProcess();
	}

	if (postLoopCallback != nullptr)
		postLoopCallback(this);
}

void ACore::Execute()
{
	CheckError(!IsInitialized(), RETURN_VOID, "Cannot execute core. Core is not initialized.");

	while (true)
		Process();
}

void ACore::SetInitializationCallback(const ACoreInitializedCallback& callback)
{
	initializationCallback = callback;
}

const ACoreInitializedCallback& ACore::GetInitializationCallback() const
{
	return initializationCallback;
}

void ACore::SetDeinitializationCallback(const ACoreDeinitializedCallback& callback)
{
	deinitializationCallback = callback;
}

const ACoreDeinitializedCallback& ACore::GetDeinitializationCallback() const
{
	return deinitializationCallback;
}

void ACore::SetPreLoopCallback(const ACorePreLoopCallback& callback)
{
	preLoopCallback = callback;
}

const ACorePreLoopCallback& ACore::GetPreLoopCallback() const
{
	return preLoopCallback;
}

void ACore::SetPostLoopCallback(const ACorePostLoopCallback& callback)
{
	postLoopCallback = callback;
}

const ACorePostLoopCallback& ACore::GetPostLoopCallback() const
{
	return postLoopCallback;
}

ACore::ACore()
{
	telegramIndex = 0;
	HIDPacketIndex = 0;

	interfaceDevice = nullptr;
	initialized = false;
	printMessages = false;
	printHIDPackets = false;
}

ACore::~ACore()
{
	if (IsInitialized())
		Deinitialize();

	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
		delete *iterator;
}