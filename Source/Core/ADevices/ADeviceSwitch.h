#pragma once

#include "ACore/ADevice.h"

class ACEMIMessageData;
class ADeviceSwitch : public ADevice
{
	private:
		ADataPoint							onOffControl;
		ADataPoint							onOffstatus;
		ADataPoint							dimLevelControl;
		ADataPoint							dimLevelStatus;

		bool								onOff;
		float								dimLevel;

		void								OnOnOffUpdated(const ACEMIMessageData& message);
		void								OnDimLevelUpdated(const ACEMIMessageData& message);


	public:
		void								SetOnOffAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffAddress() const;

		void								SetOnOffStatusAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffStatusAddress() const;

		void								SetDimLevelAddress(const AGroupAddress& address);
		const AGroupAddress&				GetDimLevelAddress() const;

		void								SetDimLevelStatusAddress(const AGroupAddress& address);
		const AGroupAddress&				GetDimLevelStatusAddress() const;


		void								SetOnOff(bool value);
		bool								GetOnOff() const;

		void								SetDim(float value);
		float								GetDimLevel() const;

											ADeviceSwitch();
		virtual								~ADeviceSwitch();
};