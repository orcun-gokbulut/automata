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

void ADevice::AddDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot add data point to device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() == nullptr, RETURN_VOID,
		"Cannot add data point to device. Data point is already added to a device. Device Name: %s. Data point name: %s.",
		GetName(),
		dataPoint->GetName());

	dataPoints.insert(dataPoints.end(), dataPoint);
	dataPoint->device = this;
}

void ADevice::RemoveDataPoint(ADataPoint* dataPoint)
{
	CheckError(dataPoint == NULL, RETURN_VOID, "Cannot remove data point from device. Data point is NULL. Device Name : % s.", GetName());
	CheckError(dataPoint->GetDevice() == this, RETURN_VOID,
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

void ADevice::TelegramReceived(const ACEMIMessage* message)
{
	if (message->GetMessageCode() == ACEMIMessageCode::Data_Received)
		return;

	const ACEMIMessageData* dataMessage = static_cast<const ACEMIMessageData*>(message);

	for (auto iterator = dataPoints.begin(); iterator != dataPoints.end(); iterator++)
	{
		ADataPoint* current = *iterator;
		if (current->GetAddress() == dataMessage->GetDestinationGroup())
		{
			switch (current->GetType())
			{
				default:
				case ADataPointType::Null:
					break;

				case ADataPointType::UINT8:
					current->SetUInt8(dataMessage->GetUInt8());
					break;

				case ADataPointType::UINT16:
					current->SetUInt16(dataMessage->GetUInt16());
					break;

				case ADataPointType::UINT32:
					current->SetUInt32(dataMessage->GetUInt32());
					break;

				case ADataPointType::UINT64:
					current->SetUInt64(dataMessage->GetUInt64());
					break;

				case ADataPointType::INT8:
					current->SetInt8(dataMessage->GetInt8());
					break;

				case ADataPointType::INT16:
					current->SetInt16(dataMessage->GetInt16());
					break;

				case ADataPointType::INT32:
					current->SetInt32(dataMessage->GetInt32());
					break;

				case ADataPointType::INT64:
					current->SetInt64(dataMessage->GetInt64());
					break;

				case ADataPointType::FLOAT:
					current->SetFloat(dataMessage->GetFloat());
					break;

				case ADataPointType::DOUBLE:
					current->SetDouble(dataMessage->GetDouble());
					break;

				case ADataPointType::BOOL:
					current->SetBoolean(dataMessage->GetBoolean());
					break;

				case ADataPointType::STRING:
					break;
			}
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
	for (auto iterator = dataPoints.begin(); iterator != dataPoints.end(); iterator++)
		delete *iterator;
}