#pragma once

#include "Common/Types.h"

#include <string>

using namespace std;

class OHAddress
{
	private:
		union
		{
			struct
			{
				uint8		b : 4;
				uint8		a : 4;
				uint8		c : 8;
			};
			uint8			byte[2];
			uint16			raw;
		};

	public:
		void			SetA(uint8 value);
		uint8			GetA() const;

		void			SetB(uint8 value);
		uint8			GetB() const;

		void			SetC(uint8 value);
		uint8			GetC() const;

		void			SetInt(uint16 value);
		uint16			GetInt() const;

		void			SetRaw(uint16 value);
		uint16			GetRaw() const;

		void			SetString(const char* value);
		string			GetString() const;


						OHAddress();
						OHAddress(const char* value);
						OHAddress(uint16 raw);
						OHAddress(uint8 a, uint8 b, uint8 c);
};
