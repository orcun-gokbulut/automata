#include "ADataPoint.h"

#include "ACore/ACore.h"
#include "ACore/ADevice.h"
#include "ACore/ADataPoint.h"
#include "ACore/ACEMIMessage.h"

#include "ACommon/AError.h"

#include <string.h>
#include <memory.h>


ADevice* ADataPoint::GetDevice() const
{
	return device;
}

ACore* ADataPoint::GetCore() const
{
	if (GetDevice() == nullptr)
		return nullptr;

	return GetDevice()->GetCore();
}

void ADataPoint::SetName(const char* name)
{
	this->name = name;
}

const char* ADataPoint::GetName() const
{
	return name.c_str();
}

std::string ADataPoint::GetPath() const
{
	if (device != nullptr)
		return device->GetName() + std::string("::") + GetName();
	else
		return GetName();
}

void ADataPoint::SetAddress(AGroupAddress address)
{
	this->address = address;
}

const AGroupAddress& ADataPoint::GetAddress() const
{
	return address;
}

void ADataPoint::SetType(ADataPointType type)
{
	this->type = type;
}

ADataPointType ADataPoint::GetType()
{
	return type;
}

bool ADataPoint::IsNull() const
{
	return type == ADataPointType::Null;
}

void ADataPoint::SetUInt8(uint8 value)
{
	CheckError(type != ADataPointType::UInt8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint8 ADataPoint::GetUInt8() const
{
	CheckError(type != ADataPointType::UInt8, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (uint8)value.integer;
}

void ADataPoint::SetUInt16(uint16 value) 
{
	CheckError(type != ADataPointType::UInt16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint16 ADataPoint::GetUInt16() const
{
	CheckError(type != ADataPointType::UInt16, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (uint16)value.integer;
}

void ADataPoint::SetUInt32(uint32 value)
{
	CheckError(type != ADataPointType::UInt32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint32 ADataPoint::GetUInt32() const
{
	CheckError(type != ADataPointType::UInt32, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (uint32)value.integer;
}

void ADataPoint::SetUInt64(uint64 value)
{
	CheckError(type != ADataPointType::UInt64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint64 ADataPoint::GetUInt64() const
{
	CheckError(type != ADataPointType::UInt64, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return value.integer;
}

void ADataPoint::SetInt8(int8 value)
{
	CheckError(type != ADataPointType::Int8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	this->value.integer = (uint64)value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int8 ADataPoint::GetInt8() const
{
	CheckError(type != ADataPointType::Int8, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (int8)value.integer;
}

void ADataPoint::SetInt16(int16 value)
{
	CheckError(type != ADataPointType::Int16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int16 ADataPoint::GetInt16() const
{
	CheckError(type != ADataPointType::Int16, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (int16)value.integer;
}

void ADataPoint::SetInt32(int32 value)
{
	CheckError(type != ADataPointType::Int32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int32 ADataPoint::GetInt32() const
{
	CheckError(type != ADataPointType::Int32, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (int32)value.integer;
}

void ADataPoint::SetInt64(int64 value)
{
	CheckError(type != ADataPointType::Int64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int64 ADataPoint::GetInt64() const
{
	CheckError(type != ADataPointType::Int64, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (int64)value.integer;
}

void ADataPoint::SetHalfFloat(float value)
{
	CheckError(type != ADataPointType::HalfFloat, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

float ADataPoint::GetHalfFloat() const
{
	return (float)value.double_;
}

void ADataPoint::SetFloat(float value)
{
	CheckError(type != ADataPointType::Float, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = (double)value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

float ADataPoint::GetFloat()
{
	CheckError(type != ADataPointType::Float, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return (float)value.double_;
}

void ADataPoint::SetDouble(double value)
{
	CheckError(type != ADataPointType::Float, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

double ADataPoint::GetDouble() const
{
	CheckError(type != ADataPointType::Float, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return value.double_;
}

void ADataPoint::SetBoolean(bool value)
{
	CheckError(type != ADataPointType::Boolean, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.boolean = value;

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

bool ADataPoint::GetBoolean() const
{
	CheckError(type != ADataPointType::Boolean, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);

	return value.boolean;
}

void ADataPoint::SetString(const char* value)
{
	CheckError(type != ADataPointType::String, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);
	CheckError(value == nullptr, RETURN_VOID, "Setting OHDataPoint value is failed. String is NULL.");
	CheckError(strlen(value) > 14, RETURN_VOID, "Setting OHDataPoint value is failed. String value length is too big. Length: %d.", strlen(value));

	strcpy_s(this->value.string_, 15, value);

	if (!insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

const char* ADataPoint::GetString()
{
	CheckError(type != ADataPointType::String, nullptr, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->type, type);
	
	return value.string_;
}

void ADataPoint::SetOnChanged(ADataPointChangedCallback callback)
{
	OnChanged = callback;
}

const ADataPointChangedCallback& ADataPoint::GetOnChanged() const
{
	return OnChanged;
}

void ADataPoint::SetOnUpdated(ADataPointUpdatedCallback callback)
{
	OnUpdated = callback;
}

const ADataPointUpdatedCallback& ADataPoint::GetOnUpdated() const
{
	return OnUpdated;
}

void ADataPoint::Process(const ACEMIMessageData& message)
{
	switch (GetType())
	{
		default:
		case ADataPointType::Null:
			break;

		case ADataPointType::UInt8:
			CheckError(message.GetPayloadSize() != sizeof(uint8), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt8, Received Size: %u.", 
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetUInt8(message.GetUInt8());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::UInt16:
			CheckError(message.GetPayloadSize() != sizeof(uint16), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt16, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetUInt16(message.GetUInt16());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::UInt32:
			CheckError(message.GetPayloadSize() != sizeof(uint32), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt32, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetUInt32(message.GetUInt32());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::UInt64:
			CheckError(message.GetPayloadSize() != sizeof(uint16), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt64, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetUInt64(message.GetUInt64());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Int8:
			CheckError(message.GetPayloadSize() != sizeof(int8), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int8, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetInt8(message.GetInt8());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Int16:
			CheckError(message.GetPayloadSize() != sizeof(int16), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int16, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetInt16(message.GetInt16());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Int32:
			CheckError(message.GetPayloadSize() != sizeof(int32), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int32, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());
			
			insideUpdateEvent = true;
			SetInt32(message.GetInt32());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Int64:
			CheckError(message.GetPayloadSize() != sizeof(int64), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int64, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetInt64(message.GetInt64());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::HalfFloat:
			CheckError(message.GetPayloadSize() != 2, RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Float, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetHalfFloat(message.GetHalfFloat());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Float:
			CheckError(message.GetPayloadSize() != sizeof(float), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Float, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetFloat(message.GetFloat());
			OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Double:
			CheckError(message.GetPayloadSize() != sizeof(double), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Double, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetDouble(message.GetDouble());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::Boolean:
			CheckError(message.GetPayloadSize() != 0, RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Boolean, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			insideUpdateEvent = true;
			SetBoolean(message.GetBoolean());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			insideUpdateEvent = false;
			break;

		case ADataPointType::String:
			break;
	}
}

void ADataPoint::Generate(ACEMIMessageData& message)
{
	ACore* core = GetCore();
	if (core != nullptr)
		message.SetSource(core->GetAddress());

	message.SetAddressType(AAddressType::GroupAddress);
	message.SetAPCI(ACEMIAPCI::GroupValueWrite);
	message.SetDestinationGroup(address);

	switch (GetType())
	{
		default:
		case ADataPointType::Null:
			break;

		case ADataPointType::UInt8:
			message.SetUInt8(GetUInt8());
			break;

		case ADataPointType::UInt16:
			message.SetUInt16(GetUInt16());
			break;

		case ADataPointType::UInt32:
			message.SetUInt32(GetUInt32());
			break;

		case ADataPointType::UInt64:
			message.SetUInt64(GetUInt64());
			break;

		case ADataPointType::Int8:
			message.SetInt8(GetInt8());
			break;

		case ADataPointType::Int16:
			message.SetInt16(GetInt16());
			break;

		case ADataPointType::Int32:
			message.SetInt32(GetInt32());
			break;

		case ADataPointType::Int64:
			message.SetInt64(GetInt64());
			break;

		case ADataPointType::HalfFloat:
			message.SetHalfFloat(GetHalfFloat());
			break;

		case ADataPointType::Float:
			message.SetFloat(GetFloat());
			break;

		case ADataPointType::Double:
			message.SetDouble(GetDouble());
			break;

		case ADataPointType::Boolean:
			message.SetBoolean(GetBoolean());
			break;

		case ADataPointType::String:
			break;
	}
}

void ADataPoint::SendUpdate()
{
	ACore* core = GetCore();
	if (core == nullptr)
		return;

	ACEMIMessageData message;
	message.SetMessageCode(ACEMIMessageCode::Data_Transmit);
	Generate(message);
	core->SendMessage(message);
}

ADataPoint::ADataPoint()
{
	device = nullptr;
	type = ADataPointType::Null;
	insideUpdateEvent = false;
	memset(&value, 0, sizeof(value));
}

ADataPoint::~ADataPoint()
{
	if (device != nullptr)
		device->UnregisterDataPoint(this);
}
