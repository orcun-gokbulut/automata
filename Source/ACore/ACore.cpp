#include "ACore.h"

#include "ADevice.h"
#include "ACommon/AError.h"

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

void ACore::Process()
{
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
		(*iterator)->Process();
}

ACore::ACore()
{

}

ACore::~ACore()
{
	for (auto iterator = devices.begin(); iterator != devices.end(); iterator++)
		delete *iterator;
}