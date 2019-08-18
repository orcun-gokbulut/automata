#include "ADevice.h"

#include "ACommon/AError.h"

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

const string ADevice::GetName() const
{
	return name;
}

const std::vector<ADataPoint*>& ADevice::GetDataPoints() const
{
	return dataPoints;
}

void ADevice::AddDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot add data point to device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() == nullptr, RETURN_VOID,
		"Cannot add data point to device. Data point is already added to a device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.insert(dataPoints.end(), dataPoint);
}

void ADevice::RemoveDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot remove data point from device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() == this, RETURN_VOID,
		"Cannot remove data point from device. Data point is not added to this device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.erase(std::find(dataPoints.begin(), dataPoints.end(), dataPoint));
}

void ADevice::Process()
{

}

ADevice::ADevice()
{
	serialNumber = 0;
}

ADevice::~ADevice()
{
	for (auto iterator = dataPoints.begin(); iterator != dataPoints.end(); iterator++)
		delete iterator._Ptr;
}