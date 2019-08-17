#include "Address.h"

#include "Common/Error.h"

#include <regex>

void OHAddress::SetA(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part A part is too big.");
	a = value;
}

uint8 OHAddress::GetA() const
{
	return a;
}

void OHAddress::SetB(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part B part is too big.");
	b = value;
}

uint8 OHAddress::GetB() const
{
	return b;
}

void OHAddress::SetC(uint8 value)
{
	c = value;
}

uint8 OHAddress::GetC() const
{
	return c;
}

void OHAddress::SetRaw(uint16 value)
{
	raw = value;
}

uint16 OHAddress::GetRaw() const
{
	return raw;
}

void OHAddress::SetInt(uint16 value)
{
	byte[1] = (uint8)(value << 8);
	byte[0] = (uint8)(value & 0xFF);
}

uint16 OHAddress::GetInt() const
{
	return ((uint8)byte[1] << 8) | (uint8)byte[0];
}

void OHAddress::SetString(const string& value)
{
	regex re(R"(^\s*([0-9]+)\s*\/\s*([0-9]+)\s*\/\s*([0-9]+)\s*$)");
	smatch match;
	
	bool result = regex_match(value, match, re);
	CheckError(!result, RETURN_VOID, "Cannot set address. Invalid address format.");

	int ta = atoi(match.str(1).c_str());
	int tb = atoi(match.str(2).c_str());
	int tc = atoi(match.str(3).c_str());

	CheckError(ta >= 16 || tb >= 16 || tc >= 256, RETURN_VOID, "Cannot set address. One or more of the address part(s) is too big.");

	a = ta;
	b = tb;
	c = tc;
}

string OHAddress::GetString() const
{
	return std::to_string(a) + "/" + std::to_string(b) + "/" + std::to_string(c);
}

OHAddress::OHAddress()
{
	byte[0] = 0x00;
	byte[1] = 0x00;
}

OHAddress::OHAddress(uint16 raw)
{
	SetRaw(raw);
}

OHAddress::OHAddress(uint8 a, uint8 b, uint8 c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}