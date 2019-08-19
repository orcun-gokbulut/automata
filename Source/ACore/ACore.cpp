#include "ACore.h"

#include "ADevice.h"
#include "ATelegram.h"

#include "ACommon/AError.h"

#include <hidapi.h>

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
		Log("Enumerating device. Manufacturer: %ls. Product: %ls. VID: %04hx. PID: %04hx.",
			currentDeviceInfo->manufacturer_string,
			currentDeviceInfo->product_string,
			currentDeviceInfo->vendor_id,
			currentDeviceInfo->product_id);

		if (currentDeviceInfo->vendor_id == 0x28c2 && currentDeviceInfo->product_id == 0x0017)
		{
			Log("USB KNX Interface device found.\n"
				"  Vendor Id:     %04hx\n"
				"  Product Id:    %04hx\n"
				"  Serial Number: %ls\n"
				"  Manufacturer:  %ls\n"
				"  Product:       %ls\n"
				"  Release:       %hx\n"
				"  Interface:     %d\n"
				"  Path:          %ls",
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
}

void ACore::RemoveDevice(ADevice* device)
{
	CheckError(device == NULL, RETURN_VOID, "Cannot remove device from core. Device is NULL.");
	CheckError(device->GetCore() == this, RETURN_VOID, "Cannot remove device from core. Device is not added to this core. Device Name: %s.", device->GetName());

	devices.erase(std::find(devices.begin(), devices.end(), device));
	device->core = nullptr;
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

	return true;
}

bool ACore::Deinitialize()
{
	Log("Deinitializing core...");

	DeinitializeDevices();
	DeinitializeInterface();

	initialized = false;

	Log("Core deinitialized.");

	return true;
}

void ACore::DispatchTelegram(const ATelegram& telegram)
{
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);
		
		if (!currentDevice->IsInitialized())
			continue;

		currentDevice->TelegramReceived(telegram);
	}
}

bool ACore::SendTelegram(const ATelegram& telegram)
{
	CheckError(!IsInitialized(), false, "Cannot send telegram. Core is not initialized.");

	uint8 buffer[1024];
	size_t size;
	telegram.Generate(buffer, size);
	
	int bytesSend = hid_write(interfaceDevice, buffer, size);
	
	CheckError(bytesSend != size, false, "Cannot send telegram.");

	return true;
}

void ACore::Process()
{
	CheckError(!IsInitialized(), RETURN_VOID, "Cannot process core. Core is not initialized.");

	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);
		if (!currentDevice->IsInitialized())
			continue;

		currentDevice->PreProcess();
	}

	uint8 buffer[1024];
	uint8* cursor = buffer;
	int bytesReceived = hid_read_timeout(interfaceDevice, buffer, 1024, 100);
	while (bytesReceived > 0)
	{
		ATelegram telegram;
		if (telegram.Process(buffer, 1024))
			break;

		DispatchTelegram(telegram);

		bytesReceived -= (int)telegram.GetSize();
		cursor = cursor + telegram.GetSize();
	}

	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
	{
		ADevice* currentDevice = (*iterator);
		if (!currentDevice->IsInitialized())
			continue;

		currentDevice->PostProcess();
	}
}

void ACore::Execute()
{
	CheckError(!IsInitialized(), RETURN_VOID, "Cannot execute core. Core is not initialized.");

	while (true)
		Process();
}

ACore::ACore()
{
	interfaceDevice = nullptr;
	initialized = false;
}

ACore::~ACore()
{
	if (IsInitialized())
		Deinitialize();

	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
		delete *iterator;
}