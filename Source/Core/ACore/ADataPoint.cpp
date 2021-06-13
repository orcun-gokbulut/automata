#include "ADataPoint.h"

#include "ACore/ACore.h"
#include "ACore/ADevice.h"
#include "ACore/ACEMIMessage.h"

#include "ACommon/AError.h"

#include <string.h>
#include <memory.h>


ADevice* ADataPoint::GetDevice() const
{
	return m_device;
}

ACore* ADataPoint::GetCore() const
{
	if (GetDevice() == nullptr)
		return nullptr;

	return GetDevice()->GetCore();
}

void ADataPoint::SetName(const char* name)
{
	this->m_name = name;
}

const char* ADataPoint::GetName() const
{
	return m_name.c_str();
}

std::string ADataPoint::GetPath() const
{
	if (m_device != nullptr)
		return m_device->GetName() + std::string("::") + GetName();
	else
		return GetName();
}

void ADataPoint::SetAddress(AGroupAddress address)
{
	this->m_address = address;
}

const AGroupAddress& ADataPoint::GetAddress() const
{
	return m_address;
}

void ADataPoint::SetType(ADataPointType type)
{
	this->m_type = type;
}

ADataPointType ADataPoint::GetType()
{
	return m_type;
}

bool ADataPoint::IsNull() const
{
	return m_type == ADataPointType::Null;
}

void ADataPoint::SetUInt8(uint8 value)
{
	CheckError(m_type != ADataPointType::UInt8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint8 ADataPoint::GetUInt8() const
{
	CheckError(m_type != ADataPointType::UInt8, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (uint8)value.m_integer;
}

void ADataPoint::SetUInt16(uint16 value)
{
	CheckError(m_type != ADataPointType::UInt16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint16 ADataPoint::GetUInt16() const
{
	CheckError(m_type != ADataPointType::UInt16, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (uint16)value.m_integer;
}

void ADataPoint::SetUInt32(uint32 value)
{
	CheckError(m_type != ADataPointType::UInt32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint32 ADataPoint::GetUInt32() const
{
	CheckError(m_type != ADataPointType::UInt32, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (uint32)value.m_integer;
}

void ADataPoint::SetUInt64(uint64 value)
{
	CheckError(m_type != ADataPointType::UInt64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

uint64 ADataPoint::GetUInt64() const
{
	CheckError(m_type != ADataPointType::UInt64, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return value.m_integer;
}

void ADataPoint::SetInt8(int8 value)
{
	CheckError(m_type != ADataPointType::Int8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	this->value.m_integer = (uint64)value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int8 ADataPoint::GetInt8() const
{
	CheckError(m_type != ADataPointType::Int8, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (int8)value.m_integer;
}

void ADataPoint::SetInt16(int16 value)
{
	CheckError(m_type != ADataPointType::Int16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = (uint64)value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int16 ADataPoint::GetInt16() const
{
	CheckError(m_type != ADataPointType::Int16, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (int16)value.m_integer;
}

void ADataPoint::SetInt32(int32 value)
{
	CheckError(m_type != ADataPointType::Int32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = (uint64)value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int32 ADataPoint::GetInt32() const
{
	CheckError(m_type != ADataPointType::Int32, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (int32)value.m_integer;
}

void ADataPoint::SetInt64(int64 value)
{
	CheckError(m_type != ADataPointType::Int64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_integer = (uint64)value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

int64 ADataPoint::GetInt64() const
{
	CheckError(m_type != ADataPointType::Int64, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (int64)value.m_integer;
}

void ADataPoint::SetHalfFloat(float value)
{
	CheckError(m_type != ADataPointType::HalfFloat, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_double_ = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

float ADataPoint::GetHalfFloat() const
{
	return (float)value.m_double_;
}

void ADataPoint::SetFloat(float value)
{
	CheckError(m_type != ADataPointType::Float, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_double_ = (double)value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

float ADataPoint::GetFloat()
{
	CheckError(m_type != ADataPointType::Float, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return (float)value.m_double_;
}

void ADataPoint::SetDouble(double value)
{
	CheckError(m_type != ADataPointType::Float, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_double_ = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

double ADataPoint::GetDouble() const
{
	CheckError(m_type != ADataPointType::Float, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return value.m_double_;
}

void ADataPoint::SetBoolean(bool value)
{
	CheckError(m_type != ADataPointType::Boolean, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);

	this->value.m_boolean = value;

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

bool ADataPoint::GetBoolean() const
{
	CheckError(m_type != ADataPointType::Boolean, 0, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return value.m_boolean;
}

void ADataPoint::SetString(const char* value)
{
	CheckError(m_type != ADataPointType::String, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->m_type, m_type);
	CheckError(value == nullptr, RETURN_VOID, "Setting OHDataPoint value is failed. String is NULL.");
	CheckError(strlen(value) > 14, RETURN_VOID, "Setting OHDataPoint value is failed. String value length is too big. Length: %d.", strlen(value));

	strcpy(this->value.m_string_, value);

	if (!m_insideUpdateEvent)
	{
		if (OnChanged != nullptr)
			OnChanged(*this);

		SendUpdate();
	}
}

const char* ADataPoint::GetString()
{
	CheckError(m_type != ADataPointType::String, nullptr, "Getting DataPoint value is failed. Type Mismatch. DataPoint: %s, Type: %d, Set Type: %d.", GetPath().c_str(), this->m_type, m_type);

	return value.m_string_;
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

			m_insideUpdateEvent = true;
			SetUInt8(message.GetUInt8());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::UInt16:
			CheckError(message.GetPayloadSize() != sizeof(uint16), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt16, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetUInt16(message.GetUInt16());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::UInt32:
			CheckError(message.GetPayloadSize() != sizeof(uint32), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt32, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetUInt32(message.GetUInt32());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::UInt64:
			CheckError(message.GetPayloadSize() != sizeof(uint16), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: UInt64, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetUInt64(message.GetUInt64());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Int8:
			CheckError(message.GetPayloadSize() != sizeof(int8), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int8, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetInt8(message.GetInt8());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Int16:
			CheckError(message.GetPayloadSize() != sizeof(int16), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int16, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetInt16(message.GetInt16());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Int32:
			CheckError(message.GetPayloadSize() != sizeof(int32), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int32, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetInt32(message.GetInt32());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Int64:
			CheckError(message.GetPayloadSize() != sizeof(int64), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Int64, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetInt64(message.GetInt64());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::HalfFloat:
			CheckError(message.GetPayloadSize() != 2, RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Float, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetHalfFloat(message.GetHalfFloat());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Float:
			CheckError(message.GetPayloadSize() != sizeof(float), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Float, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetFloat(message.GetFloat());
			OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Double:
			CheckError(message.GetPayloadSize() != sizeof(double), RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Double, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetDouble(message.GetDouble());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
			break;

		case ADataPointType::Boolean:
			CheckError(message.GetPayloadSize() != 0, RETURN_VOID,
				"Cannot update Datapoint. Type size mismatch. DataPoint %s, Type: Boolean, Received Size: %u.",
				GetPath().c_str(), message.GetPayloadSize());

			m_insideUpdateEvent = true;
			SetBoolean(message.GetBoolean());
			if (OnUpdated != nullptr)
				OnUpdated(*this, message);
			m_insideUpdateEvent = false;
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
	message.SetDestinationGroup(m_address);

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
	message.SetMessageCode(ACEMIMessageCode::DataTransmitRequest);
	Generate(message);

	core->SendMessage(message);
}

ADataPoint::ADataPoint()
{
	m_device = nullptr;
	m_type = ADataPointType::Null;
	m_insideUpdateEvent = false;
	memset(&value, 0, sizeof(value));
}

ADataPoint::~ADataPoint()
{
	if (m_device != nullptr)
		m_device->UnregisterDataPoint(this);
}
