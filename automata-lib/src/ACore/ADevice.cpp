#include "ADevice.h"

#include "ADataPoint.h"
#include "ACEMIMessage.h"
#include "ACommon/AError.h"


ACore* ADevice::GetCore() const
{
	return m_core;
}

void ADevice::SetSerialNumber(uint32 number)
{
	m_serialNumber = number;
}

int ADevice::GetSerialNumber() const
{
	return m_serialNumber;
}

void ADevice::SetName(const string& name)
{
	this->m_name = name;
}

const char* ADevice::GetName() const
{
	return m_name.c_str();
}

const std::vector<ADataPoint*>& ADevice::GetDataPoints() const
{
	return m_dataPoints;
}

void ADevice::RegisterDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot add data point to device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() != nullptr, RETURN_VOID,
		"Cannot add data point to device. Data point is already added to a device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	m_dataPoints.insert(m_dataPoints.end(), dataPoint);
	dataPoint->m_device = this;
}

void ADevice::UnregisterDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot remove data point from device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() != this, RETURN_VOID,
		"Cannot remove data point from device. Data point is not added to this device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	m_dataPoints.erase(std::find(m_dataPoints.begin(), m_dataPoints.end(), dataPoint));
	dataPoint->m_device = nullptr;
}

bool ADevice::IsInitialized()
{
	return m_initialized;
}

bool ADevice::Initialize()
{
	m_initialized = true;

	return true;
}

bool ADevice::Deinitialize()
{
	m_initialized = false;

	return true;
}

void ADevice::TelegramReceived(const ACEMIMessage& message)
{
	if (message.GetMessageCode() != ACEMIMessageCode::DataReceived)
		return;

	const ACEMIMessageData& dataMessage = static_cast<const ACEMIMessageData&>(message);
	if (dataMessage.GetAPCI() == ACEMIAPCI::GroupValueWrite ||
		dataMessage.GetAPCI() == ACEMIAPCI::GroupValueRead ||
		dataMessage.GetAPCI() == ACEMIAPCI::GroupValueResponse)
	{
		for (auto iterator = m_dataPoints.begin(); iterator != m_dataPoints.end(); iterator++)
		{
			ADataPoint* current = *iterator;
			if (current->GetAddress().GetRaw() != 0x0000 && current->GetAddress() == dataMessage.GetDestinationGroup())
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
	m_core = nullptr;
	m_serialNumber = 0;
	m_initialized = false;
}

ADevice::~ADevice()
{
	CheckError(m_dataPoints.size() != 0, RETURN_VOID, "Cannot deconstruct Device. There still registered data points available. Device Name: %s.", GetName());
}
