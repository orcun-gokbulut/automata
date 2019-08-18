#pragma once

#include "ACommon/ATypes.h"
#include "ACore/AAddress.h"

#include <string>

using namespace std;

enum class OHDataPointType
{
	Null,
	UINT8,
	UINT16,
	UINT32,
	UINT64,
	INT8,
	INT16,
	INT32,
	INT64,
	FLOAT,
	DOUBLE,
	BOOL,
	STRING
};

class ADevice;

class ADataPoint
{
	private:
		ADevice*			device;
		string				name;
		AAddress			address;
		OHDataPointType		type;
		union
		{
			uint64			integer;
			double			double_;
			bool			boolean;
			char			string_[15];
		} value;

	public:
		ADevice*			GetDevice() const;

		void				SetName(const char* value);
		const char*			GetName() const;

		void				SetAddress(AAddress address);
		AAddress			GetAddress() const;

		void				SetType(OHDataPointType type);
		OHDataPointType		GetType();

		bool				IsNull() const;

		void				SetUInt8(uint8 value);
		uint8				GetUInt8() const;
		void				SetUInt16(uint16 value);
		uint16				GetUInt16() const;
		void				SetUInt32(uint32 value);
		uint32				GetUInt32() const;
		void				SetUInt64(uint64 value);
		uint64				GetUInt64() const;

		void				SetInt8(int8 value);
		int8				GetInt8() const;
		void				SetInt16(int16 value);
		int16				GetInt16() const;
		void				SetInt32(int32 value);
		int32				GetInt32() const;
		void				SetInt64(int64 value);
		int64				GetInt64() const;

		void				SetFloat(float value);
		float				GetFloat();

		void				SetDouble(double value);
		double				GetDouble() const;

		void				SetBoolean(bool value);
		bool				GetBoolean() const;

		void				SetString(const char* value);
		const char*			GetString();

							ADataPoint();
};