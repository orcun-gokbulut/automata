#include "ADeviceRelay.h"

#include <url_parser.h>
#include <httplib.h>

void ADeviceRelay::OnOnOffControlUpdated(const ACEMIMessageData& message)
{
    url_parser_url url;
    parse_url(const_cast<char*>(onURL.c_str()), false, &url);

    httplib::Client client(url.host, url.port);
    std::shared_ptr<httplib::Response> response = client.Get(url.path);
    if (response->status == 200)
        onOffStatus.SetBoolean(onOffControl.GetBoolean());

    free_parsed_url(&url);
}

void ADeviceRelay::SetOnOffAddress(const AGroupAddress& address)
{
    this->onOffControl.SetAddress(address);
}

const AGroupAddress& ADeviceRelay::GetOnOffAddress() const
{
    return this->onOffControl.GetAddress();
}

void ADeviceRelay::SetOnURL(const string& URL)
{
    this->onURL = URL;
}

const string& ADeviceRelay::GetOnURL() const
{
    return this->onURL;
}

void ADeviceRelay::SetOffURL(const string& URL)
{
    this->offURL = URL;
}

const string& ADeviceRelay::GetOffURL() const
{
    return this->offURL;
}

void ADeviceRelay::SetOnOffStatusAddress(const AGroupAddress& address)
{
    this->onOffStatus.SetAddress(address);
}

const AGroupAddress& ADeviceRelay::GetOnOffStatusAddress() const
{
    return this->onOffStatus.GetAddress();
}

ADeviceRelay::ADeviceRelay()
{
	onOffControl.SetType(ADataPointType::Boolean);
	onOffControl.SetOnUpdated([this](const ADataPoint& datapoint, const ACEMIMessageData& message) {this->OnOnOffControlUpdated(message);});
	RegisterDataPoint(&onOffControl);

	onOffStatus.SetType(ADataPointType::Boolean);
	RegisterDataPoint(&onOffStatus);
}

ADeviceRelay::~ADeviceRelay()
{
	UnregisterDataPoint(&onOffControl);
	UnregisterDataPoint(&onOffStatus);
}
