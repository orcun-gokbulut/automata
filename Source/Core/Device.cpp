#include "Device.h"

#include "Common/Error.h"

void OHDevice::SetSerialNumber(uint32 number)
{
	serialNumber = number;
}

int OHDevice::GetSerialNumber() const
{
	return serialNumber;
}

void OHDevice::SetName(const string& name)
{
	this->name = name;
}

const string OHDevice::GetName() const
{
	return name;
}

const std::vector<OHDataPoint*>& OHDevice::GetDataPoints() const
{
	return dataPoints;
}

void OHDevice::AddDataPoint(OHDataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot add data point to device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() == nullptr, RETURN_VOID,
		"Cannot add data point to device. Data point is already added to a device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.insert(dataPoints.end(), dataPoint);
}

void OHDevice::RemoveDataPoint(OHDataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot remove data point from device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() == this, RETURN_VOID,
		"Cannot remove data point from device. Data point is not added to this device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.erase(std::find(dataPoints.begin(), dataPoints.end(), dataPoint));
}

void OHDevice::Process()
{

}

OHDevice::OHDevice()
{
	serialNumber = 0;
}

OHDevice::~OHDevice()
{
	for (auto iterator = dataPoints.begin(); iterator != dataPoints.end(); iterator++)
		delete iterator._Ptr;
}