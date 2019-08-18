#pragma once

#include "ACommon/ATypes.h"

#include <string>

using namespace std;

class AAddress
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


						AAddress();
						AAddress(const char* value);
						AAddress(uint16 raw);
						AAddress(uint8 a, uint8 b, uint8 c);
};
