#include "AHIDReport.h"

#include "AHIDProtocol.h"

#include "ACommon/AEndian.h"
#include "ACommon/AError.h"

#include <sstream>
#include <memory.h>

void AHIDReport::SetIndex(uint64 index) const
{
	this->m_index = index;
}

uint64 AHIDReport::GetIndex() const
{
	return m_index;
}

void AHIDReport::SetReportID(uint8 id)
{
	m_reportID = id;
}

uint8 AHIDReport::GetReportID() const
{
	return m_reportID;
}

void AHIDReport::SetStartPacket(bool flag)
{
	m_startPacket = flag;
}

bool AHIDReport::GetStartPacket() const
{
	return m_startPacket;
}

void AHIDReport::SetEndPacket(bool flag)
{
	m_endPacket = flag;
}

bool AHIDReport::GetEndPacket() const
{
	return m_endPacket;
}

void AHIDReport::SetPartialPacket(bool flag)
{
	m_partialPacket = flag;
}

bool AHIDReport::GetPartialPacket() const
{
	return m_partialPacket;
}

void AHIDReport::SetSequenceNumber(uint8 number)
{
	m_sequenceNumber = number;
}

uint8 AHIDReport::GetSequenceNumber() const
{
	return m_sequenceNumber;
}

void AHIDReport::SetProtocolVersion(uint8 version)
{
	m_protocolVersion = version;
}

uint8 AHIDReport::GetProtocolVersion() const
{
	return m_protocolVersion;
}

void AHIDReport::SetDataSize(uint8 size)
{
	m_dataSize = size;
}

uint8 AHIDReport::GetDataSize() const
{
	return m_dataSize;
}

void AHIDReport::SetData(const void* data, uint8 size)
{
	if (data == nullptr)
		SetDataSize(0);

	if (size != 0)
		SetDataSize(size);

	memcpy(this->m_data, data, GetDataSize());
}

void AHIDReport::AddData(const void* data, uint8 size)
{
	if (data == nullptr || size == 0)
		return;

	memcpy(this->m_data + m_dataSize, data, size);
	m_dataSize += size;
}

const void* AHIDReport::GetData() const
{
	return m_data;
}

void AHIDReport::SetProtocolId(AHIDProtocolId id)
{
	m_protocolId = id;
}

AHIDProtocolId AHIDReport::GetProtocolId() const
{
	return m_protocolId;
}

void AHIDReport::SetEMIId(uint8 id)
{
	m_EMIId = id;
}

uint8 AHIDReport::GetEMIId() const
{
	return m_EMIId;
}

void AHIDReport::SetManufacturerCode(uint16 code)
{
	m_manufacturerCode = code;
}

uint16 AHIDReport::GetManufacturerCode() const
{
	return m_manufacturerCode;
}

void AHIDReport::Generate(void* buffer, uint8& size) const
{
	size = 0;

	AHIDReportHeader* header = (AHIDReportHeader*)buffer;
	header->m_reportID = GetReportID();
	header->m_startPacket = GetStartPacket();
	header->m_endPacket = GetEndPacket();
	header->m_partialPacket = GetPartialPacket();
	header->m_reserved = 0;
	header->m_sequenceNumber = GetSequenceNumber();
	header->m_dataLength = 8 + GetDataSize();
	size += sizeof(AHIDReportHeader);

	AHIDReportBody* body = (AHIDReportBody*)((uint8*)buffer + size);
	body->m_protocolVersion = GetProtocolVersion();
	body->m_headerLength = 8;
	body->m_bodyLenght = bswap_16((uint16)GetDataSize());
	body->m_EMIId = GetEMIId();
	body->m_protocolId = (uint8)GetProtocolId();
	body->m_manufacturerCode = GetManufacturerCode();
	size += sizeof(AHIDReportBody);

	void* data = (uint8*)buffer + size;
	memcpy(data, GetData(), GetDataSize());

	size += GetDataSize();
}

bool AHIDReport::Process(const void* buffer, uint8 size)
{
	AHIDReportHeader* header = (AHIDReportHeader*)buffer;
	SetReportID(header->m_reportID);
	SetStartPacket(header->m_startPacket);
	SetEndPacket(header->m_endPacket);
	SetPartialPacket(header->m_partialPacket);
	SetSequenceNumber(header->m_sequenceNumber);

	AHIDReportBody* body = (AHIDReportBody*)((uint8*)buffer + sizeof(AHIDReportHeader));
	SetProtocolVersion(body->m_protocolVersion);
	SetProtocolId((AHIDProtocolId)body->m_protocolId);
	SetEMIId(body->m_EMIId);
	SetManufacturerCode(body->m_manufacturerCode);

	uint8* dataBuffer = (uint8*)body + sizeof(AHIDReportBody);
	SetData(dataBuffer, bswap_16(body->m_bodyLenght));

	return true;
}

void AHIDReport::Reset()
{
	m_index = 0;
	m_reportID = 0x01;
	m_startPacket = true;
	m_endPacket = true;
	m_partialPacket = false;
	m_sequenceNumber = 0x01;
	m_protocolVersion = 0x00;
	m_protocolId = AHIDProtocolId::KNXTunnel;
	m_EMIId = 0x03;
	m_manufacturerCode = 0x0000;
	m_dataSize = 0;

	memset(m_data, 0, sizeof(m_data));
}

std::string AHIDReport::ToString() const
{
	std::stringstream output;
	output << "HID Packet #" << std::to_string((int)m_index) << "\n";
	output << "  Report ID: " << std::to_string((int)m_reportID) << "\n";
	output << "  Start Packet: " << (m_startPacket ? "True" : "False") << "\n";
	output << "  End Packet: " << (m_endPacket ? "True" : "False") << "\n";
	output << "  Partial Packet: " << (m_partialPacket ? "True" : "False") << "\n";
	output << "  Sequence Number: " << std::to_string((int)m_sequenceNumber) << "\n";
	output << "  Protocol Version: " << std::to_string(m_protocolVersion) << "\n";

	output << "  Protocol Id: ";
	switch (m_protocolId)
	{
		case AHIDProtocolId::KNXTunnel:
			output << "KNX Tunnel (0x01)";
			break;
		case AHIDProtocolId::BusAccessServerFeatureService:
			output << "Bus Access (0x0F)";
			break;
		default:
			output << "UNKNOWN (" << std::to_string((int)m_protocolId) << ")";
	}
	output << "\n";

	output << "  EMI ID: ";
	switch (m_EMIId)
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
			output << "UNKNOWN (" << std::to_string((int)m_EMIId) << ")";
			break;
	}
	output << "\n";

	output << "  Manufacturer Code: " << std::to_string((int)m_manufacturerCode) << "\n";
	output << "  Data Size: " << (int)m_dataSize << "\n";

	output << "  Data: ";
	for (uint8 i = 0; i < m_dataSize; i++)
	{
		char hexOutput[3];
		toHex(m_data[i], hexOutput);
		output << hexOutput << " ";
	}
	output << "\n";

	return output.str();
}

AHIDReport::AHIDReport()
{
	Reset();
}
