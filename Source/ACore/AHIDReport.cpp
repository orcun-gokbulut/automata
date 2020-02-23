#include "AHIDReport.h"

#include "AHIDProtocol.h"

#include <sstream>
#include <ACommon\AError.h>

void AHIDReport::SetIndex(uint64 index)
{
	this->index = index;
}

uint64 AHIDReport::GetIndex() const
{
	return index;
}

void AHIDReport::SetReportID(uint8 id)
{
	reportID = id;
}

uint8 AHIDReport::GetReportID() const
{
	return reportID;
}

void AHIDReport::SetStartPacket(bool flag)
{
	startPacket = flag;
}

bool AHIDReport::GetStartPacket() const
{
	return startPacket;
}

void AHIDReport::SetEndPacket(bool flag)
{
	endPacket = flag;
}

bool AHIDReport::GetEndPacket() const
{
	return endPacket;
}

void AHIDReport::SetPartialPacket(bool flag)
{
	partialPacket = flag;
}

bool AHIDReport::GetPartialPacket() const
{
	return partialPacket;
}

void AHIDReport::SetSequenceNumber(uint8 number)
{
	sequenceNumber = number;
}

uint8 AHIDReport::GetSequenceNumber() const
{
	return sequenceNumber;
}

void AHIDReport::SetProtocolVersion(uint8 version)
{
	protocolVersion = version;
}

uint8 AHIDReport::GetProtocolVersion() const
{
	return protocolVersion;
}

void AHIDReport::SetDataSize(uint8 size)
{
	dataSize = size;
}

uint8 AHIDReport::GetDataSize() const
{
	return dataSize;
}

void AHIDReport::SetData(const void* data, uint8 size)
{
	if (data == nullptr)
		SetDataSize(0);

	if (size != 0)
		SetDataSize(size);

	memcpy(this->data, data, GetDataSize());
}

const void* AHIDReport::GetData() const
{
	return data;
}

size_t AHIDReport::GetSize() const
{
	return GetDataSize() + 11;
}

void AHIDReport::SetProtocolId(uint8 id)
{
	protocolId = id;
}

uint8 AHIDReport::GetProtocolId() const
{
	return protocolId;
}

void AHIDReport::SetEMIId(uint8 id)
{
	EMIId = id;
}

uint8 AHIDReport::GetEMIId() const
{
	return EMIId;
}

void AHIDReport::SetManufacturerCode(uint16 code)
{
	manufacturerCode = code;
}

uint16 AHIDReport::GetManufacturerCode() const
{
	return manufacturerCode;
}

void AHIDReport::Generate(void* buffer, uint8& size) const
{
	AHIDReportHeader* header = (AHIDReportHeader*)buffer;
	header->reportID = GetReportID();
	header->startPacket = GetStartPacket();
	header->endPacket = GetEndPacket();
	header->partialPacket = GetPartialPacket();
	header->reserved = 0;
	header->sequenceNumber = GetSequenceNumber();
	header->dataLength = 8 + GetDataSize();

	AHIDReportBody* body = (AHIDReportBody*)((uint8*)buffer + sizeof(AHIDReportHeader));
	body->protocolVersion = GetProtocolVersion();
	body->headerLength = 8;
	body->bodyLenght = (uint16)GetDataSize() << 8;
	body->EMIId = GetEMIId();
	body->protocolId = GetProtocolId();
	body->manufacturerCode = GetManufacturerCode();
	
	void* data = (AHIDReportBody*)((uint8*)buffer + 8);
	memcpy(data, GetData(), GetDataSize());
	
	size = 8 + GetDataSize();
}

bool AHIDReport::Process(const void* buffer, uint8 size)
{
	AHIDReportHeader* header = (AHIDReportHeader*)buffer;
	AHIDReportBody* body = (AHIDReportBody*)((uint8*)buffer + 3);

	SetReportID(header->reportID);
	SetStartPacket(header->startPacket);
	SetEndPacket(header->endPacket);
	SetPartialPacket(header->partialPacket);
	SetSequenceNumber(header->sequenceNumber);
	SetProtocolVersion(body->protocolVersion);
	SetProtocolId(body->protocolId);
	SetEMIId(body->EMIId);
	SetManufacturerCode(body->manufacturerCode);

	uint8* dataBuffer = (uint8*)body + 8;
	SetData(dataBuffer, body->bodyLenght >> 8);

	return true;
}

std::string AHIDReport::Print() const
{
	std::stringstream output;
	output << "HID Packet #" << (int)index << "\n";
	output << "  Report ID: " << (int)reportID << "\n";
	output << "  Start Packet: " << (startPacket ? "True" : "False") << "\n";
	output << "  End Packet: " << (endPacket ? "True" : "False") << "\n";
	output << "  Partial Packet: " << (partialPacket ? "True" : "False") << "\n";
	output << "  Sequence Number: " << (int)sequenceNumber << "\n";
	output << "  Protocol Version: " << protocolVersion << "\n";

	output << "  Protocol Id: ";
	switch (protocolId)
	{
		case 0x01:
			output << "KNX Tunnel (0x01)";
			break;
		case 0x02:
			output << "M-Bus Tunnel (0x02 - Not Supported)";
			break;
		case 0x03:
			output << "Batibus Tunnel (0x03 - Not Supported)";
			break;
		case 0x0F:
			output << "Bus Access (0x0F)";
			break;
		default:
			output << "UNKNOWN" << (int)protocolId << ")";
	}
	output << "\n";

	output << "  EMI  ID: ";
	switch (EMIId)
	{
		case 0x01:
			output << "EMI1 (0x01)";
			break;
		case 0x02:
			output << "EMI2 (0x02)";
			break;
		case 0x03:
			output << "cEMI (0x03)";
			break;
		default:
			output << "UNKNOWN (" << (int) EMIId << ")";
			break;
	}
	output << "\n";
	
	output << "  Manufacturer Code: " << (int)manufacturerCode << "\n";
	output << "  Data Size: " << (int)dataSize << "\n";
	
	output << "  Data: ";
	for (uint8 i = 0; i < dataSize; i++)
	{
		char hexOutput[3];
		toHex(data[i], hexOutput);
		output << hexOutput << " ";
	}
	output << "\n";

	return output.str();
}

AHIDReport::AHIDReport()
{
	index = 0;
	reportID = 0x01;
	startPacket = true;
	endPacket = true;
	partialPacket = false;
	sequenceNumber = 0x01;
	protocolVersion = 0x00;
	protocolId = 0x00;
	EMIId = 0x00;
	manufacturerCode = 0x0000;
	dataSize = 0;

	memset(data, 0, sizeof(data));
}
