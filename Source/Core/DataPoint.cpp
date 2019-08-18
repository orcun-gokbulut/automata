#include "DataPoint.h"

#include "Common/Error.h"

#include <string.h>
#include <memory.h>

ADevice* ADataPoint::GetDevice() const
{
	return device;
}

void ADataPoint::SetName(const char* name)
{
	this->name = name;
}

const char* ADataPoint::GetName() const
{
	return name.c_str();
}

void ADataPoint::SetAddress(AAddress address)
{
	this->address = address;
}
AAddress ADataPoint::GetAddress() const
{
	return address;
}

void ADataPoint::SetType(OHDataPointType type)
{
	this->type = type;
}

OHDataPointType ADataPoint::GetType()
{
	return type;
}

bool ADataPoint::IsNull() const
{
	return type == OHDataPointType::Null;
}

void ADataPoint::SetUInt8(uint8 value)
{
	CheckError(type == OHDataPointType::UINT8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint8 ADataPoint::GetUInt8() const
{
	CheckError(type == OHDataPointType::UINT8, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (uint8)value.integer;
}

void ADataPoint::SetUInt16(uint16 value) 
{
	CheckError(type == OHDataPointType::UINT16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint16 ADataPoint::GetUInt16() const
{
	CheckError(type == OHDataPointType::UINT16, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (uint16)value.integer;
}

void ADataPoint::SetUInt32(uint32 value)
{
	CheckError(type == OHDataPointType::UINT32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint32 ADataPoint::GetUInt32() const
{
	CheckError(type == OHDataPointType::UINT32, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (uint32)value.integer;
}

void ADataPoint::SetUInt64(uint64 value)
{
	CheckError(type == OHDataPointType::UINT64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint64 ADataPoint::GetUInt64() const
{
	CheckError(type == OHDataPointType::UINT64, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return value.integer;
}

void ADataPoint::SetInt8(int8 value)
{
	CheckError(type == OHDataPointType::INT8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int8 ADataPoint::GetInt8() const
{
	CheckError(type == OHDataPointType::INT8, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int8)value.integer;
}

void ADataPoint::SetInt16(int16 value)
{
	CheckError(type == OHDataPointType::INT16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int16 ADataPoint::GetInt16() const
{
	CheckError(type == OHDataPointType::INT16, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int16)value.integer;
}

void ADataPoint::SetInt32(int32 value)
{
	CheckError(type == OHDataPointType::INT32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int32 ADataPoint::GetInt32() const
{
	CheckError(type == OHDataPointType::INT32, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int32)value.integer;
}

void ADataPoint::SetInt64(int64 value)
{
	CheckError(type == OHDataPointType::INT64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int64 ADataPoint::GetInt64() const
{
	CheckError(type == OHDataPointType::INT64, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int64)value.integer;
}

void ADataPoint::SetFloat(float value)
{
	CheckError(type == OHDataPointType::FLOAT, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = (double)value;
}

float ADataPoint::GetFloat()
{
	CheckError(type == OHDataPointType::FLOAT, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (float)value.double_;
}

void ADataPoint::SetDouble(double value)
{
	CheckError(type == OHDataPointType::FLOAT, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = value;
}

double ADataPoint::GetDouble() const
{
	CheckError(type == OHDataPointType::FLOAT, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return value.double_;
}

void ADataPoint::SetBoolean(bool value)
{
	CheckError(type == OHDataPointType::BOOL, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.boolean = value;
}

bool ADataPoint::GetBoolean() const
{
	CheckError(type == OHDataPointType::BOOL, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return value.boolean;
}

void ADataPoint::SetString(const char* value)
{
	CheckError(type == OHDataPointType::STRING, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);
	CheckError(value == nullptr, RETURN_VOID, "Setting OHDataPoint value is failed. String is NULL.");
	CheckError(strlen(value) > 14, RETURN_VOID, "Setting OHDataPoint value is failed. String value length is too big. Length: %d.", strlen(value));

	strcpy_s(this->value.string_, 15, value);
}

const char* ADataPoint::GetString()
{
	CheckError(type == OHDataPointType::STRING, nullptr, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);
	
	return value.string_;
}

ADataPoint::ADataPoint()
{
	device = nullptr;
	memset(&value, 0, sizeof(value));
}