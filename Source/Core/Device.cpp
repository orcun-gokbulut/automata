#include "Device.h"

void Device::SetName(const string& name)
{
	this->name = name;
}

const string Device::GetName() const
{
	return name;
}