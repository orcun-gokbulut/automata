#include "DataPoint.h"

#include "Common/Error.h"

#include <string.h>
#include <memory.h>

void OHDataPoint::SetName(const char* name)
{
	this->name = name;
}

const char* OHDataPoint::GetName() const
{
	return name.c_str();
}

void OHDataPoint::SetAddress(OHAddress address)
{
	this->address = address;
}
OHAddress OHDataPoint::GetAddress() const
{
	return address;
}

void OHDataPoint::SetType(OHDataPointType type)
{
	this->type = type;
}

OHDataPointType OHDataPoint::GetType()
{
	return type;
}

bool OHDataPoint::IsNull() const
{
	return type == OHDataPointType::Null;
}

void OHDataPoint::SetUInt8(uint8 value)
{
	CheckError(type == OHDataPointType::UINT8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint8 OHDataPoint::GetUInt8() const
{
	CheckError(type == OHDataPointType::UINT8, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (uint8)value.integer;
}

void OHDataPoint::SetUInt16(uint16 value) 
{
	CheckError(type == OHDataPointType::UINT16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint16 OHDataPoint::GetUInt16() const
{
	CheckError(type == OHDataPointType::UINT16, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (uint16)value.integer;
}

void OHDataPoint::SetUInt32(uint32 value)
{
	CheckError(type == OHDataPointType::UINT32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint32 OHDataPoint::GetUInt32() const
{
	CheckError(type == OHDataPointType::UINT32, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (uint32)value.integer;
}

void OHDataPoint::SetUInt64(uint64 value)
{
	CheckError(type == OHDataPointType::UINT64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = value;
}

uint64 OHDataPoint::GetUInt64() const
{
	CheckError(type == OHDataPointType::UINT64, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return value.integer;
}

void OHDataPoint::SetInt8(int8 value)
{
	CheckError(type == OHDataPointType::INT8, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int8 OHDataPoint::GetInt8() const
{
	CheckError(type == OHDataPointType::INT8, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int8)value.integer;
}

void OHDataPoint::SetInt16(int16 value)
{
	CheckError(type == OHDataPointType::INT16, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int16 OHDataPoint::GetInt16() const
{
	CheckError(type == OHDataPointType::INT16, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int16)value.integer;
}

void OHDataPoint::SetInt32(int32 value)
{
	CheckError(type == OHDataPointType::INT32, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int32 OHDataPoint::GetInt32() const
{
	CheckError(type == OHDataPointType::INT32, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int32)value.integer;
}

void OHDataPoint::SetInt64(int64 value)
{
	CheckError(type == OHDataPointType::INT64, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.integer = (uint64)value;
}

int64 OHDataPoint::GetInt64() const
{
	CheckError(type == OHDataPointType::INT64, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (int64)value.integer;
}

void OHDataPoint::SetFloat(float value)
{
	CheckError(type == OHDataPointType::FLOAT, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = (double)value;
}

float OHDataPoint::GetFloat()
{
	CheckError(type == OHDataPointType::FLOAT, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return (float)value.double_;
}

void OHDataPoint::SetDouble(double value)
{
	CheckError(type == OHDataPointType::FLOAT, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.double_ = value;
}

double OHDataPoint::GetDouble() const
{
	CheckError(type == OHDataPointType::FLOAT, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return value.double_;
}

void OHDataPoint::SetBoolean(bool value)
{
	CheckError(type == OHDataPointType::BOOL, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);

	this->value.boolean = value;
}

bool OHDataPoint::GetBoolean() const
{
	CheckError(type == OHDataPointType::BOOL, 0, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);

	return value.boolean;
}

void OHDataPoint::SetString(const char* value)
{
	CheckError(type == OHDataPointType::STRING, RETURN_VOID, "Setting OHDataPoint value is failed. Type Mismatch. Type: %d, Set Type: %d.", this->type, type);
	CheckError(value == nullptr, RETURN_VOID, "Setting OHDataPoint value is failed. String is NULL.");
	CheckError(strlen(value) > 14, RETURN_VOID, "Setting OHDataPoint value is failed. String value length is too big. Length: %d.", strlen(value));

	strcpy_s(this->value.string_, 15, value);
}

const char* OHDataPoint::GetString()
{
	CheckError(type == OHDataPointType::STRING, nullptr, "Getting OHDataPoint value is failed. Type Mismatch. Data Point Type: %d, Set Type: %d.", this->type, type);
	
	return value.string_;
}

OHDataPoint::OHDataPoint()
{
	memset(&value, 0, sizeof(value));
}