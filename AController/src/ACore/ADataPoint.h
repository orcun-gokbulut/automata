#pragma once

#include "ACommon/ATypes.h"
#include "ACore/AAddress.h"

#include <string>
#include <functional>

enum class ADataPointType
{
	Null,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Int8,
	Int16,
	Int32,
	Int64,
	HalfFloat,
	Float,
	Double,
	Boolean,
	String
};

class ACore;
class ADevice;
class ACEMIMessageData;

#define ADataPointChangedCallback std::function<void (const ADataPoint&)>
#define ADataPointUpdatedCallback std::function<void (const ADataPoint&, const ACEMIMessageData&)>

class ADataPoint
{
	friend class ADevice;
	private:
		ADevice*							device;
		std::string							name;
		AGroupAddress						address;
		ADataPointType						type;
		union
		{
			uint64							integer;
			double							double_;
			bool							boolean;
			char							string_[15];
		} value;

		bool								insideUpdateEvent;
		ADataPointChangedCallback			OnChanged;
		ADataPointUpdatedCallback			OnUpdated;

	public:
		ADevice*							GetDevice() const;
		ACore*								GetCore() const;

		void								SetName(const char* value);
		const char*							GetName() const;

		std::string							GetPath() const;

		void								SetAddress(AGroupAddress address);
		const AGroupAddress&				GetAddress() const;

		void								SetType(ADataPointType type);
		ADataPointType						GetType();

		bool								IsNull() const;

		void								SetUInt8(uint8 value);
		uint8								GetUInt8() const;
		void								SetUInt16(uint16 value);
		uint16								GetUInt16() const;
		void								SetUInt32(uint32 value);
		uint32								GetUInt32() const;
		void								SetUInt64(uint64 value);
		uint64								GetUInt64() const;

		void								SetInt8(int8 value);
		int8								GetInt8() const;
		void								SetInt16(int16 value);
		int16								GetInt16() const;
		void								SetInt32(int32 value);
		int32								GetInt32() const;
		void								SetInt64(int64 value);
		int64								GetInt64() const;

		void								SetHalfFloat(float value);
		float								GetHalfFloat() const;
		void								SetFloat(float value);
		float								GetFloat();
		void								SetDouble(double value);
		double								GetDouble() const;

		void								SetBoolean(bool value);
		bool								GetBoolean() const;

		void								SetString(const char* value);
		const char*							GetString();

		void								SetOnChanged(ADataPointChangedCallback callback);
		const ADataPointChangedCallback&	GetOnChanged() const;

		void								SetOnUpdated(ADataPointUpdatedCallback callback);
		const ADataPointUpdatedCallback&	GetOnUpdated() const;

		void								Process(const ACEMIMessageData& message);
		void								Generate(ACEMIMessageData& message);
		void								SendUpdate();

											ADataPoint();
											~ADataPoint();
};