#pragma once

#include "ACommon/ATypes.h"

#include <string>

using namespace std;

enum class AAddressType : uint8
{
	IndividualAddress = 0,
	GroupAddress = 1
};

#pragma pack(push)
#pragma pack(1)

class AGroupAddress
{
	private:
		union
		{
			struct
			{
				uint8			b : 3;
				uint8			a : 5;
				uint8			c : 8;
			};
			uint8				byte[2];
			uint16				raw;
		};

	public:
		void					SetA(uint8 value);
		uint8					GetA() const;

		void					SetB(uint8 value);
		uint8					GetB() const;

		void					SetC(uint8 value);
		uint8					GetC() const;

		void					SetRaw(uint16 value);
		uint16					GetRaw() const;

		bool					SetString(const char* value);
		string					GetString() const;

		bool					operator==(const AGroupAddress& other) const;
		bool					operator!=(const AGroupAddress& other) const;

								AGroupAddress();
								AGroupAddress(const char* value);
								AGroupAddress(uint16 raw);
								AGroupAddress(uint8 a, uint8 b, uint8 c);
};

class AIndividualAddress
{
	private:
		union
		{
			struct
			{
				uint8			b : 4;
				uint8			a : 4;
				uint8			c : 8;
			};
			uint8				byte[2];
			uint16				raw;
		};

	public:
		void					SetA(uint8 value);
		uint8					GetA() const;

		void					SetB(uint8 value);
		uint8					GetB() const;

		void					SetC(uint8 value);
		uint8					GetC() const;

		void					SetRaw(uint16 value);
		uint16					GetRaw() const;

		bool					SetString(const char* value);
		string					GetString() const;


		bool					operator==(const AIndividualAddress& other) const;
		bool					operator!=(const AIndividualAddress& other) const;

								AIndividualAddress();
								AIndividualAddress(const char* value);
								AIndividualAddress(uint16 raw);
								AIndividualAddress(uint8 a, uint8 b, uint8 c);
};

#pragma pack(pop)