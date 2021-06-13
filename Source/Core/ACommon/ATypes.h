#pragma once

#define int8 char
#define int16 short
#define int32 int
#define int64 long long
#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long long

void toHex(uint8 value, char* hexOutput);
void toHex16(uint16 value, char* hexOutput);

uint16 toHalf(float value);
float toFloat(uint16 value);
