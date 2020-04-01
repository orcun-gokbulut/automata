#pragma once

#include "ADevice.h"

class ACEMIMessageData;
void ADeviceRemoteRelay: public ADevice
{
	private:
		ADataPoint							onOffControl;
		ADataPoint							onOffstatus;

        string                              onURL;
        string                              offURL;

		void								OnOnOffUpdated(const ACEMIMessageData& message);

	public:
		void								SetOnOffAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffAddress() const;

        void								SetOnOffStatusAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffStatusAddress() const;

        void                                SetOnURL(const string URL);
        const string&                       GetOnURL() const;

        void                                SetOffURL(const string URL);
        const string&                       GetOffURL() const;

		void								SetOnOffStatusAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffStatusAddress() const;

        									ADeviceSwitch();
		virtual								~ADeviceSwitch();
};