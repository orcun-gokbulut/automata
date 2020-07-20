#pragma once

#include "ACore/ADevice.h"

class ACEMIMessageData;
class ADeviceRelay : public ADevice
{
	private:
		ADataPoint							onOffControl;
		ADataPoint							onOffStatus;

        std::string                         onURL;
        std::string                         offURL;

		void								OnOnOffControlUpdated(const ACEMIMessageData& message);

	public:
		void								SetOnOffAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffAddress() const;

        void								SetOnOffStatusAddress(const AGroupAddress& address);
		const AGroupAddress&				GetOnOffStatusAddress() const;

        void                                SetOnURL(const std::string& URL);
        const std::string&                  GetOnURL() const;

        void                                SetOffURL(const std::string& URL);
        const std::string&                  GetOffURL() const;

        									ADeviceRelay();
		virtual								~ADeviceRelay();
};