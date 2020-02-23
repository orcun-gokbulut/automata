#include "AAddress.h"

#include "ACommon/AError.h"

#include <regex>

void AGroupAddress::SetA(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part A part is too big.");
	a = value;
}

uint8 AGroupAddress::GetA() const
{
	return a;
}

void AGroupAddress::SetB(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part B part is too big.");
	b = value;
}

uint8 AGroupAddress::GetB() const
{
	return b;
}

void AGroupAddress::SetC(uint8 value)
{
	c = value;
}

uint8 AGroupAddress::GetC() const
{
	return c;
}

void AGroupAddress::SetRaw(uint16 value)
{
	raw = value;
}

uint16 AGroupAddress::GetRaw() const
{
	return raw;
}

bool AGroupAddress::SetString(const char* value)
{
	regex re(R"(^\s*([0-9]+)\s*\/\s*([0-9]+)\s*\/\s*([0-9]+)\s*$)");
	smatch match;
	
	string valueStr = value;
	bool result = regex_match(valueStr, match, re);
	CheckError(!result, false, "Cannot set address. Invalid address format.");

	int ta = atoi(match.str(1).c_str());
	int tb = atoi(match.str(2).c_str());
	int tc = atoi(match.str(3).c_str());

	CheckError(ta >= 32 || tb >= 8 || tc >= 256, false, "Cannot set address. One or more of the address part(s) is too big.");

	a = ta;
	b = tb;
	c = tc;

	return true;
}

string AGroupAddress::GetString() const
{
	return std::to_string(a) + "/" + std::to_string(b) + "/" + std::to_string(c);
}

bool AGroupAddress::operator==(const AGroupAddress& other) const
{
	return raw == other.raw;
}

bool AGroupAddress::operator!=(const AGroupAddress& other) const
{
	return raw != other.raw;
}


AGroupAddress::AGroupAddress()
{
	byte[0] = 0x00;
	byte[1] = 0x00;
}

AGroupAddress::AGroupAddress(const char* value)
{
	SetString(value);
}

AGroupAddress::AGroupAddress(uint16 raw)
{
	SetRaw(raw);
}

AGroupAddress::AGroupAddress(uint8 a, uint8 b, uint8 c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}


// AIndividualAddress
////////////////////////////////////////////////////////////////////////////////////////////////////

void AIndividualAddress::SetA(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part A part is too big.");
	a = value;
}

uint8 AIndividualAddress::GetA() const
{
	return a;
}

void AIndividualAddress::SetB(uint8 value)
{
	CheckError(value >= 16, RETURN_VOID, "Cannot set address. Address part B part is too big.");
	b = value;
}

uint8 AIndividualAddress::GetB() const
{
	return b;
}

void AIndividualAddress::SetC(uint8 value)
{
	c = value;
}

uint8 AIndividualAddress::GetC() const
{
	return c;
}

void AIndividualAddress::SetRaw(uint16 value)
{
	raw = value;
}

uint16 AIndividualAddress::GetRaw() const
{
	return raw;
}

bool AIndividualAddress::SetString(const char* value)
{
	regex re(R"(^\s*([0-9]+)\s*\.\s*([0-9]+)\s*\.\s*([0-9]+)\s*$)");
	smatch match;

	string valueStr = value;
	bool result = regex_match(valueStr, match, re);
	CheckError(!result, false, "Cannot set address. Invalid address format.");

	int ta = atoi(match.str(1).c_str());
	int tb = atoi(match.str(2).c_str());
	int tc = atoi(match.str(3).c_str());

	CheckError(ta >= 16 || tb >= 16 || tc >= 256, false, "Cannot set address. One or more of the address part(s) is too big.");

	a = ta;
	b = tb;
	c = tc;

	return true;
}

string AIndividualAddress::GetString() const
{
	return std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c);
}

bool AIndividualAddress::operator==(const AIndividualAddress& other) const
{
	return raw == other.raw;
}

bool AIndividualAddress::operator!=(const AIndividualAddress& other) const
{
	return raw != other.raw;
}


AIndividualAddress::AIndividualAddress()
{
	byte[0] = 0x00;
	byte[1] = 0x00;
}

AIndividualAddress::AIndividualAddress(const char* value)
{
	SetString(value);
}

AIndividualAddress::AIndividualAddress(uint16 raw)
{
	SetRaw(raw);
}

AIndividualAddress::AIndividualAddress(uint8 a, uint8 b, uint8 c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}