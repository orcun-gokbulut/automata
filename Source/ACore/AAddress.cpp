#include "AAddress.h"

#include "ACommon/AError.h"

#include <regex>

void AAddress::SetA(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part A part is too big.");
	a = value;
}

uint8 AAddress::GetA() const
{
	return a;
}

void AAddress::SetB(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part B part is too big.");
	b = value;
}

uint8 AAddress::GetB() const
{
	return b;
}

void AAddress::SetC(uint8 value)
{
	c = value;
}

uint8 AAddress::GetC() const
{
	return c;
}

void AAddress::SetRaw(uint16 value)
{
	raw = value;
}

uint16 AAddress::GetRaw() const
{
	return raw;
}

void AAddress::SetInt(uint16 value)
{
	byte[1] = (uint8)(value << 8);
	byte[0] = (uint8)(value & 0xFF);
}

uint16 AAddress::GetInt() const
{
	return ((uint8)byte[1] << 8) | (uint8)byte[0];
}

void AAddress::SetString(const char* value)
{
	regex re(R"(^\s*([0-9]+)\s*\/\s*([0-9]+)\s*\/\s*([0-9]+)\s*$)");
	smatch match;
	
	string valueStr = value;
	bool result = regex_match(valueStr, match, re);
	CheckError(!result, RETURN_VOID, "Cannot set address. Invalid address format.");

	int ta = atoi(match.str(1).c_str());
	int tb = atoi(match.str(2).c_str());
	int tc = atoi(match.str(3).c_str());

	CheckError(ta >= 16 || tb >= 16 || tc >= 256, RETURN_VOID, "Cannot set address. One or more of the address part(s) is too big.");

	a = ta;
	b = tb;
	c = tc;
}

string AAddress::GetString() const
{
	return std::to_string(a) + "/" + std::to_string(b) + "/" + std::to_string(c);
}

AAddress::AAddress()
{
	byte[0] = 0x00;
	byte[1] = 0x00;
}

AAddress::AAddress(const char* value)
{
	SetString(value);
}

AAddress::AAddress(uint16 raw)
{
	SetRaw(raw);
}

AAddress::AAddress(uint8 a, uint8 b, uint8 c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}