#include "AHIDReport.h"

#include "AHIDProtocol.h"

#include "ACommon/AEndian.h"
#include "ACommon/AError.h"

#include <sstream>

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

	memcpy_s(this->data, sizeof(this->data), data, GetDataSize());
}

void AHIDReport::AddData(const void* data, uint8 size)
{
	if (data == nullptr || size == 0)
		return;

	memcpy(this->data + dataSize, data, size);
	dataSize += size;
}

const void* AHIDReport::GetData() const
{
	return data;
}

void AHIDReport::SetProtocolId(AHIDProtocolId id)
{
	protocolId = id;
}

AHIDProtocolId AHIDReport::GetProtocolId() const
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
	size = 0;
	
	AHIDReportHeader* header = (AHIDReportHeader*)buffer;
	header->reportID = GetReportID();
	header->startPacket = GetStartPacket();
	header->endPacket = GetEndPacket();
	header->partialPacket = GetPartialPacket();
	header->reserved = 0;
	header->sequenceNumber = GetSequenceNumber();
	header->dataLength = 8 + GetDataSize();
	size += sizeof(AHIDReportHeader);

	AHIDReportBody* body = (AHIDReportBody*)((uint8*)buffer + size);
	body->protocolVersion = GetProtocolVersion();
	body->headerLength = 8;
	body->bodyLenght = bswap_16((uint16)GetDataSize());
	body->EMIId = GetEMIId();
	body->protocolId = (uint8)GetProtocolId();
	body->manufacturerCode = GetManufacturerCode();
	size += sizeof(AHIDReportBody);

	void* data = (uint8*)buffer + size;
	memcpy(data, GetData(), GetDataSize());
	
	size += GetDataSize();
}

bool AHIDReport::Process(const void* buffer, uint8 size)
{
	AHIDReportHeader* header = (AHIDReportHeader*)buffer;
	SetReportID(header->reportID);
	SetStartPacket(header->startPacket);
	SetEndPacket(header->endPacket);
	SetPartialPacket(header->partialPacket);
	SetSequenceNumber(header->sequenceNumber);

	AHIDReportBody* body = (AHIDReportBody*)((uint8*)buffer + sizeof(AHIDReportHeader));
	SetProtocolVersion(body->protocolVersion);
	SetProtocolId((AHIDProtocolId)body->protocolId);
	SetEMIId(body->EMIId);
	SetManufacturerCode(body->manufacturerCode);

	uint8* dataBuffer = (uint8*)body + sizeof(AHIDReportBody);
	SetData(dataBuffer, bswap_16(body->bodyLenght));

	return true;
}

void AHIDReport::Reset()
{
	index = 0;
	reportID = 0x01;
	startPacket = true;
	endPacket = true;
	partialPacket = false;
	sequenceNumber = 0x01;
	protocolVersion = 0x00;
	protocolId = AHIDProtocolId::KNXTunnel;
	EMIId = 0x03;
	manufacturerCode = 0x0000;
	dataSize = 0;

	memset(data, 0, sizeof(data));
}

std::string AHIDReport::ToString() const
{
	std::stringstream output;
	output << "HID Packet #" << std::to_string((int)index) << "\n";
	output << "  Report ID: " << std::to_string((int)reportID) << "\n";
	output << "  Start Packet: " << (startPacket ? "True" : "False") << "\n";
	output << "  End Packet: " << (endPacket ? "True" : "False") << "\n";
	output << "  Partial Packet: " << (partialPacket ? "True" : "False") << "\n";
	output << "  Sequence Number: " << std::to_string((int)sequenceNumber) << "\n";
	output << "  Protocol Version: " << std::to_string(protocolVersion) << "\n";

	output << "  Protocol Id: ";
	switch (protocolId)
	{
		case AHIDProtocolId::KNXTunnel:
			output << "KNX Tunnel (0x01)";
			break;
		case AHIDProtocolId::BusAccessServerFeatureService:
			output << "Bus Access (0x0F)";
			break;
		default:
			output << "UNKNOWN (" << std::to_string((int)protocolId) << ")";
	}
	output << "\n";

	output << "  EMI ID: ";
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
			output << "UNKNOWN (" << std::to_string((int)EMIId) << ")";
			break;
	}
	output << "\n";
	
	output << "  Manufacturer Code: " << std::to_string((int)manufacturerCode) << "\n";
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
	Reset();
}
