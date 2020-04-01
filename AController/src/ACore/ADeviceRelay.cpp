#include "ADeviceRelay.h"

void ADeviceRelay::OnOnOffUpdated(const ACEMIMessageData& message)
{

}

void ADeviceRelay::SetOnOffAddress(const AGroupAddress& address)
{
    this->onOffAddress = address;
}

const AGroupAddress& ADeviceRelay::GetOnOffAddress() const
{
    return this->onOffAddress;
}

voidA DeviceRelay::SetOnURL(const string URL)
{

}

const string& ADeviceRelay::GetOnURL() const
{

}

void ADeviceRelay::SetOffURL(const string URL)
{

}

const string& ADeviceRelay::GetOffURL() const
{

}

void ADeviceRelay::SetOnOffStatusAddress(const AGroupAddress& address)
{

}

const AGroupAddress& ADeviceRelay::GetOnOffStatusAddress() const
{

}

ADeviceRelay::ADeviceRelay()
{

}

ADeviceRelay::~ADeviceRelay()
{

}