#include "ADevice.h"

#include "ADataPoint.h"
#include "ACEMIMessage.h"
#include "ACommon/AError.h"


ACore* ADevice::GetCore() const
{
	return core;
}

void ADevice::SetSerialNumber(uint32 number)
{
	serialNumber = number;
}

int ADevice::GetSerialNumber() const
{
	return serialNumber;
}

void ADevice::SetName(const string& name)
{
	this->name = name;
}

const char* ADevice::GetName() const
{
	return name.c_str();
}

const std::vector<ADataPoint*>& ADevice::GetDataPoints() const
{
	return dataPoints;
}

void ADevice::RegisterDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot add data point to device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() != nullptr, RETURN_VOID,
		"Cannot add data point to device. Data point is already added to a device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.insert(dataPoints.end(), dataPoint);
	dataPoint->device = this;
}

void ADevice::UnregisterDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot remove data point from device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() != this, RETURN_VOID,
		"Cannot remove data point from device. Data point is not added to this device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.erase(std::find(dataPoints.begin(), dataPoints.end(), dataPoint));
	dataPoint->device = nullptr;
}

bool ADevice::IsInitialized()
{
	return initialized;
}

bool ADevice::Initialize()
{
	initialized = true;

	return true;
}

bool ADevice::Deinitialize()
{
	initialized = false;

	return true;
}

void ADevice::TelegramReceived(const ACEMIMessage& message)
{
	if (message.GetMessageCode() == ACEMIMessageCode::Data_Received)
		return;

	const ACEMIMessageData& dataMessage = static_cast<const ACEMIMessageData&>(message);
	if (dataMessage.GetAPCI() == ACEMIAPCI::GroupValueWrite ||
		dataMessage.GetAPCI() == ACEMIAPCI::GroupValueRead ||
		dataMessage.GetAPCI() == ACEMIAPCI::GroupValueResponse)
	{
		for (auto iterator = dataPoints.begin(); iterator != dataPoints.end(); iterator++)
		{
			ADataPoint* current = *iterator;
			if (current->GetAddress() == dataMessage.GetDestinationGroup())
				current->Process(dataMessage);
		}
	}
}

void ADevice::DataPointChanged(const ADataPoint& dataPoint)
{

}

void ADevice::PreProcess()
{

}

void ADevice::PostProcess()
{

}

ADevice::ADevice()
{
	core = nullptr;
	serialNumber = 0;
	initialized = false;
}

ADevice::~ADevice()
{
	CheckError(dataPoints.size() != 0, RETURN_VOID, "Cannot deconstruct Device. There still registered data points available. Device Name: %s.", GetName());
}