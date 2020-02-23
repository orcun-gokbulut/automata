#include "ATypes.h"

void toHex(uint8 value, char* hexOutput)
{
	uint8 temp = (value >> 4);
	if (temp >= 10)
		hexOutput[0] = 'A' + temp - 10;
	else
		hexOutput[0] = '0' + temp;

	temp = (value & 0x0F);
	if (temp >= 10)
		hexOutput[1] = 'A' + temp - 10;
	else
		hexOutput[1] = '0' + temp;

	hexOutput[2] = '\0';
}

void toHex16(uint16 value, char* hexOutput)
{
	toHex(value & 0x00FF, hexOutput);
	toHex((value & 0xFF00) >> 8, hexOutput + 2);
	hexOutput[4] = '\0';
}