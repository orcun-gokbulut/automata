#include "ADeviceRelay.h"

#include <httplib.h>
#include <uriparser/Uri.h>

class url 
{
    public:
        std::string protocol_, host_, path_, query_;
    
        void parse(const std::string& url_s);
};

void url::parse(const string& url_s)
{
    const string prot_end("://");
    string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
                                           prot_end.begin(), prot_end.end());
    protocol_.reserve(distance(url_s.begin(), prot_i));
    transform(url_s.begin(), prot_i,
              back_inserter(protocol_),
              ptr_fun<int,int>(tolower)); // protocol is icase
    if( prot_i == url_s.end() )
        return;
    advance(prot_i, prot_end.length());
    string::const_iterator path_i = find(prot_i, url_s.end(), '/');
    host_.reserve(distance(prot_i, path_i));
    transform(prot_i, path_i,
              back_inserter(host_),
              ptr_fun<int,int>(tolower)); // host is icase
    string::const_iterator query_i = find(path_i, url_s.end(), '?');
    path_.assign(path_i, query_i);
    if( query_i != url_s.end() )
        ++query_i;
    query_.assign(query_i, url_s.end());
}

void ADeviceRelay::OnOnOffControlUpdated(const ACEMIMessageData& message)
{
    url urlTemp;
    urlTemp.parse(onOffControl.GetBoolean() ? onURL.c_str() : offURL.c_str());
  
    httplib::Client client(urlTemp.host_, 80);
    std::shared_ptr<httplib::Response> response = client.Get(urlTemp.path_.c_str());
    if (response->status == 200)
        onOffStatus.SetBoolean(onOffControl.GetBoolean());
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

}

const AGroupAddress& ADeviceRelay::GetOnOffStatusAddress() const
{

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