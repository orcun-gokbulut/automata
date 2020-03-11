#include "ADeviceSwitch.h"

#include "ACEMIMessage.h"
#include "ACommon/AError.h"

void ADeviceSwitch::OnOnOffUpdated(const ACEMIMessageData& message)
{
	bool status = message.GetBoolean();
	if (GetOnOff() == status)
		return;

	Log("Light switch changed. Device: %s, Value: %s, Address: %s.", GetName(), status ? "On" : "Off", message.GetDestinationGroup().GetString().c_str());

	SetOnOff(status);
}

void ADeviceSwitch::OnDimLevelUpdated(const ACEMIMessageData& message)
{
	float status = (float)message.GetUInt8() / 255.0f;
	if (GetDimLevel() == status)
		return;

	Log("Light dim level changed. Device: %s, Value: %f, Address: %s.", GetName(), status, message.GetDestinationGroup().GetString().c_str());

	SetDim(status);
}

void ADeviceSwitch::SetOnOffAddress(const AGroupAddress& address)
{
	onOffControl.SetAddress(address);
}

const AGroupAddress& ADeviceSwitch::GetOnOffAddress() const
{
	return onOffControl.GetAddress();
}

void ADeviceSwitch::SetOnOffStatusAddress(const AGroupAddress& address)
{
	onOffstatus.SetAddress(address);
}

const AGroupAddress& ADeviceSwitch::GetOnOffStatusAddress() const
{
	return onOffstatus.GetAddress();
}

void ADeviceSwitch::SetDimLevelAddress(const AGroupAddress& address)
{
	dimLevelControl.SetAddress(address);
}

const AGroupAddress& ADeviceSwitch::GetDimLevelAddress() const
{
	return dimLevelControl.GetAddress();
}

void ADeviceSwitch::SetDimLevelStatusAddress(const AGroupAddress& address)
{
	dimLevelStatus.SetAddress(address);
}

const AGroupAddress& ADeviceSwitch::GetDimLevelStatusAddress() const
{
	return dimLevelStatus.GetAddress();
}

void ADeviceSwitch::SetOnOff(bool value)
{
	if (onOff == value)
		return;

	onOff = value;
	onOffControl.SetBoolean(value);
}

bool ADeviceSwitch::GetOnOff() const
{
	return onOff;
}

void ADeviceSwitch::SetDim(float value)
{
	if (dimLevel == value)
		return;

	dimLevel = value;
	dimLevelControl.SetUInt8(value * 100.0f);
}

float ADeviceSwitch::GetDimLevel() const
{
	return dimLevel;
}

ADeviceSwitch::ADeviceSwitch()
{
	onOff = false;
	dimLevel = 0.0f;

	onOffControl.SetType(ADataPointType::Boolean);
	onOffControl.SetOnUpdated([this](const ADataPoint& datapoint, const ACEMIMessageData& message) {this->OnOnOffUpdated(message);});
	RegisterDataPoint(&onOffControl);

	onOffstatus.SetType(ADataPointType::Boolean);
	onOffstatus.SetOnUpdated([this](const ADataPoint& datapoint, const ACEMIMessageData& message) {this->OnOnOffUpdated(message);});
	RegisterDataPoint(&onOffstatus);

	dimLevelControl.SetType(ADataPointType::UInt8);
	dimLevelControl.SetOnUpdated([this](const ADataPoint& datapoint, const ACEMIMessageData& message) {this->OnDimLevelUpdated(message);});
	RegisterDataPoint(&dimLevelControl);

	dimLevelStatus.SetType(ADataPointType::UInt8);
	dimLevelStatus.SetOnUpdated([this](const ADataPoint& datapoint, const ACEMIMessageData& message) {this->OnDimLevelUpdated(message);});
	RegisterDataPoint(&dimLevelStatus);
}

ADeviceSwitch::~ADeviceSwitch()
{
	UnregisterDataPoint(&onOffControl);
	UnregisterDataPoint(&onOffstatus);
	UnregisterDataPoint(&dimLevelControl);
	UnregisterDataPoint(&dimLevelStatus);
}