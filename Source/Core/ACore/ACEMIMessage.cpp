#include "ACEMIMessage.h"

#include "ACEMIProtocol.h"

#include "ACommon/AError.h"
#include "ACommon/AEndian.h"

#include <sstream>
#include <memory.h>

//  ACEMIMessage
////////////////////////////////////////////////////////////////////////////////////

void ACEMIMessage::SetIndex(uint64 index) const
{
	this->m_index = index;
}

uint64 ACEMIMessage::GetIndex() const
{
	return m_index;
}

void ACEMIMessage::SetMessageCode(ACEMIMessageCode code)
{
	m_messageCode = code;
}

ACEMIMessageCode ACEMIMessage::GetMessageCode() const
{
	return m_messageCode;
}

void ACEMIMessage::Generate(void* buffer, uint8& size) const
{
	ACEMIHeader* header = (ACEMIHeader*)buffer;
	header->messageCode = (uint8)GetMessageCode();
	size = sizeof(ACEMIHeader);
}

size_t ACEMIMessage::Process(const void* buffer, size_t size)
{
	ACEMIHeader* header = (ACEMIHeader*)buffer;
	SetMessageCode((ACEMIMessageCode)header->messageCode);

	return sizeof(ACEMIHeader);
}

std::string ACEMIMessage::ToString() const
{
	stringstream output;
	output << "cEMI Message #" << m_index << "\n";

	output << "  Message Code: ";
	switch (GetMessageCode())
	{
		case ACEMIMessageCode::DataReceived:
			output << "Data Received";
			break;

		case ACEMIMessageCode::DataTransmitRequest:
			output << "Data Transmit Request";
			break;

		case ACEMIMessageCode::DataTransmitConfirmation:
			output << "Data Transmit Confirmation";
			break;

		case ACEMIMessageCode::PropertyReadRequest:
			output << "Property Read Request";
			break;

		case ACEMIMessageCode::PropertyReadConfirmation:
			output << "Property Read Confirmation";
			break;

		case ACEMIMessageCode::PropertyWriteRequest:
			output << "Property Write Request";
			break;

		case ACEMIMessageCode::PropertyWriteConfirmation:
			output << "Property Write Confirmation";
			break;

		default:
			output << "UNKNOWN";

	}

	char hexOutput[3];
	toHex((uint8)GetMessageCode(), hexOutput);
	output << " (0x" << hexOutput << ")\n";

	return output.str();
}

ACEMIMessage::ACEMIMessage()
{
	m_index = 0;
	m_messageCode = ACEMIMessageCode::DataTransmitRequest;
}

ACEMIMessage::~ACEMIMessage()
{

}


//  ACEMIMessageGeneric
////////////////////////////////////////////////////////////////////////////////////

void ACEMIMessageGeneric::SetAdditionalInfo(const void* data, uint8 size)
{
	if (size != 0)
		memcpy(m_additionalInfo, data, size);
	m_additionalInfoSize = size;
}

const void* ACEMIMessageGeneric::GetAdditionalInfo() const
{
	return m_additionalInfo;
}

uint8 ACEMIMessageGeneric::GetAdditionalInfoSize() const
{
	return m_additionalInfoSize;
}

void ACEMIMessageGeneric::Generate(void* buffer, uint8& size) const
{
	ACEMIMessage::Generate(buffer, size);

	ACEMIPacketHeaderGeneric* header = (ACEMIPacketHeaderGeneric*)((uint8*)buffer + size);
	header->additionalInfoSize = GetAdditionalInfoSize();
	if (header->additionalInfoSize != 0)
		memcpy((uint8*)buffer + sizeof(header), GetAdditionalInfo(), GetAdditionalInfoSize());

	size += sizeof(ACEMIPacketHeaderGeneric) + GetAdditionalInfoSize();
}

size_t ACEMIMessageGeneric::Process(const void* buffer, size_t size)
{
	size_t offset = ACEMIMessage::Process(buffer, size);
	if (offset == 0)
		return 0;

	ACEMIPacketHeaderGeneric* header = (ACEMIPacketHeaderGeneric*)((uint8*)buffer + offset);
	offset += sizeof(ACEMIPacketHeaderGeneric);

	SetAdditionalInfo((uint8*)header + sizeof(ACEMIHeader), header->additionalInfoSize);
	offset += header->additionalInfoSize;

	return offset;
}

std::string ACEMIMessageGeneric::ToString() const
{
	stringstream output;

	output << ACEMIMessage::ToString();

	output << "  Additional Info Size: " << std::to_string(GetAdditionalInfoSize()) << "\n";
	output << "  Additional Info: ";
	uint8* data = (uint8*)GetAdditionalInfo();
	uint8 dataSize = GetAdditionalInfoSize();
	for (uint8 i = 0; i < dataSize; i++)
	{
		char hexOutput[3];
		toHex(data[i], hexOutput);
		output << hexOutput << " ";
	}
	output << "\n";

	return output.str();
}

ACEMIMessageGeneric::ACEMIMessageGeneric()
{
	memset(m_additionalInfo, 0, sizeof(m_additionalInfo));
	m_additionalInfoSize = 0;
}

ACEMIMessageGeneric::~ACEMIMessageGeneric()
{

}


//  ACEMIMessageData
////////////////////////////////////////////////////////////////////////////////////

void ACEMIMessageData::SetFrameFormat(ACEMIFrameFormat format)
{
	m_frameFormat = format;
}

ACEMIFrameFormat ACEMIMessageData::GetFrameFormat() const
{
	return m_frameFormat;
}

void ACEMIMessageData::SetRepeatFlag(bool flag)
{
	m_repeatFlag = flag;
}

bool ACEMIMessageData::GetRepeatFlag() const
{
	return m_repeatFlag;
}

void ACEMIMessageData::SetBroadcastType(ACEMIBroadcastType type)
{
	m_systemBroadcast = type;
}

ACEMIBroadcastType ACEMIMessageData::GetBroadcastType() const
{
	return m_systemBroadcast;
}

void ACEMIMessageData::SetPriority(ACEMIPriority priority)
{
	this->m_priority = priority;
}

ACEMIPriority ACEMIMessageData::GetPriority() const
{
	return m_priority;
}

void ACEMIMessageData::SetAcknowledgeRequestFlag(bool flag)
{
	m_acknowledgeRequestFlag = flag;
}

bool ACEMIMessageData::GetAcknowledgeRequestFlag() const
{
	return m_acknowledgeRequestFlag;
}

void ACEMIMessageData::SetConfirmFlag(bool flag)
{
	m_confirmFlag = flag;
}

bool ACEMIMessageData::GetConfirmFlag() const
{
	return m_confirmFlag;
}

void ACEMIMessageData::SetHopCount(uint8 count)
{
	m_hopCount = count;
}

uint8 ACEMIMessageData::GetHopCount() const
{
	return m_hopCount;
}

void ACEMIMessageData::SetExtendedFrameFormat(uint8 format)
{
	m_extendedFrameFormat = format;
}

uint8 ACEMIMessageData::GetExtendedFrameFormat() const
{
	return m_extendedFrameFormat;
}

void ACEMIMessageData::SetAddressType(AAddressType type)
{
	m_destAddressType = type;
}

AAddressType ACEMIMessageData::GetAddressType() const
{
	return m_destAddressType;
}

void ACEMIMessageData::SetSource(const AIndividualAddress& address)
{
	m_source = address;
}

const AIndividualAddress& ACEMIMessageData::GetSource() const
{
	return m_source;
}

void ACEMIMessageData::SetDestinationIndividual(const AIndividualAddress& address)
{
	m_destinationIndividual = address;
}

const AIndividualAddress& ACEMIMessageData::GetDestinationInduvidual() const
{
	return m_destinationIndividual;
}

void ACEMIMessageData::SetDestinationGroup(const AGroupAddress& address)
{
	m_destinationGroup = address;
}

const AGroupAddress& ACEMIMessageData::GetDestinationGroup() const
{
	return m_destinationGroup;
}

void ACEMIMessageData::SetTPCI(uint8 value)
{
	m_tpci = value;
}

uint8 ACEMIMessageData::GetTPCI() const
{
	return m_tpci;
}

void ACEMIMessageData::SetAPCI(ACEMIAPCI value)
{
	m_apci = value;
}

ACEMIAPCI ACEMIMessageData::GetAPCI() const
{
	return m_apci;
}

void ACEMIMessageData::SetFirstPayload(uint8 payload)
{
	m_firstPayload = payload;
}

uint8 ACEMIMessageData::GetFirstPayload() const
{
	return m_firstPayload;
}

void ACEMIMessageData::SetPayload(const void* data, uint8 size)
{
	if (data == nullptr || size == 0)
	{
		memset(m_payload, 0, sizeof(m_payload));
	}
	else
	{
		memset(m_payload + size, 0, sizeof(m_payload) - size);
		memcpy(m_payload, data, size);
	}

	m_payloadSize = size;
}

const void* ACEMIMessageData::GetPayload() const
{
	return m_payload;
}

uint8 ACEMIMessageData::GetPayloadSize() const
{
	return m_payloadSize;
}

void ACEMIMessageData::SetUInt8(uint8 value)
{
	*(uint8*)m_payload = value;
	m_payloadSize = 1;
}

uint8 ACEMIMessageData::GetUInt8() const
{
	return *(int8*)m_payload;
}

void ACEMIMessageData::SetUInt16(uint16 value)
{
	*(uint16*)m_payload = bswap_16(value);
	m_payloadSize = 2;
}

uint16 ACEMIMessageData::GetUInt16() const
{
	return bswap_16(*(int16*)m_payload);
}

void ACEMIMessageData::SetUInt32(uint32 value)
{
	*(uint32*)m_payload = bswap_32(value);
	m_payloadSize = 4;
}

uint32 ACEMIMessageData::GetUInt32() const
{
	return bswap_32(*(uint32*)m_payload);
}

void ACEMIMessageData::SetUInt64(uint64 value)
{
	*(uint64*)m_payload = bswap_64(value);
	m_payloadSize = 8;
}

uint64 ACEMIMessageData::GetUInt64() const
{
	return bswap_64(*(uint64*)m_payload);
}

void ACEMIMessageData::SetInt8(int8 value)
{
	*(int8*)m_payload = value;
	m_payloadSize = 1;
}

int8 ACEMIMessageData::GetInt8() const
{
	return *(int8*)m_payload;
}

void ACEMIMessageData::SetInt16(int16 value)
{
	*(int16*)m_payload = (int16)bswap_16(value);
	m_payloadSize = 2;
}

int16 ACEMIMessageData::GetInt16() const
{
	return (int16)bswap_16(*(int16*)m_payload);
}

void ACEMIMessageData::SetInt32(int32 value)
{
	*(int32*)m_payload = (int32)bswap_32(value);
	m_payloadSize = 4;
}

int32 ACEMIMessageData::GetInt32() const
{
	return (int32)bswap_32(*(int32*)m_payload);
}

void ACEMIMessageData::SetInt64(int64 value)
{
	*(int64*)m_payload = (int64)bswap_64(value);
	m_payloadSize = 8;
}

int64 ACEMIMessageData::GetInt64() const
{
	return (int64)bswap_64(*(int64*)m_payload);
}

void ACEMIMessageData::SetHalfFloat(float value)
{
	*(uint16*)m_payload = toHalf(value);
	m_payloadSize = 2;
}

float ACEMIMessageData::GetHalfFloat() const
{
	return toFloat(*(uint16*)m_payload);
}

void ACEMIMessageData::SetFloat(float value)
{
	*(float*)m_payload = value;
	m_payloadSize = 4;
}

float ACEMIMessageData::GetFloat() const
{
	return *(float*)m_payload;
}

void ACEMIMessageData::SetDouble(double value)
{
	*(double*)m_payload = value;
	m_payloadSize = 8;
}

double ACEMIMessageData::GetDouble() const
{
	return *(double*)m_payload;
}

void ACEMIMessageData::SetBoolean(bool value)
{
	SetFirstPayload((uint8)value);
}

bool ACEMIMessageData::GetBoolean() const
{
	return (bool)m_firstPayload;
}

uint8 ACEMIMessageData::GetSize() const
{
	return GetPayloadSize() + A_TELEGRAM_MIN_SIZE;
}

void ACEMIMessageData::Generate(void* buffer, uint8& size) const
{
	ACEMIMessageGeneric::Generate(buffer, size);

	ACEMIPacketData* packet = (ACEMIPacketData*)((uint8*)buffer + size);
	packet->m_control.m_frameFormat = (uint8)GetFrameFormat();
	packet->m_control.m_reserved = 0;
	packet->m_control.m_repeatFlag = GetRepeatFlag();
	packet->m_control.m_systemBroadcast = (uint8)GetBroadcastType();
	packet->m_control.m_priority = (uint8)GetPriority();
	packet->m_control.m_acknowledgeRequestFlag = GetAcknowledgeRequestFlag();
	packet->m_control.m_confirmFlag = GetConfirmFlag();
	packet->m_control.m_destinationAddresType = (uint8)GetAddressType();
	packet->m_control.m_hopCount = GetHopCount();
	packet->m_control.m_extendedFrameFormat = GetExtendedFrameFormat();
	packet->m_source = m_source.GetRaw();
	if (GetAddressType() == AAddressType::IndividualAddress)
		packet->m_destination = m_destinationIndividual.GetRaw();
	else
		packet->m_destination = m_destinationGroup.GetRaw();

	packet->m_length = m_payloadSize + 1;
	packet->m_tapci0 = m_tpci << 2 | ((uint16)m_apci & 0x0300);
	packet->m_tapci1 = ((uint16)m_apci & 0x00FF);

	ACEMIAPCI trimmedACPI = GetAPCI();
	if (trimmedACPI == ACEMIAPCI::GroupValueRead || trimmedACPI == ACEMIAPCI::GroupValueWrite || trimmedACPI == ACEMIAPCI::GroupValueResponse)
		packet->m_tapci1 = packet->m_tapci1 | GetFirstPayload();

	size += sizeof(ACEMIPacketData);

	uint8* bytes = (uint8*)buffer;
	if (m_payloadSize != 0)
		memcpy(bytes + size, m_payload, m_payloadSize);

	size += m_payloadSize;
}

size_t ACEMIMessageData::Process(const void* buffer, size_t size)
{
	size_t offset = ACEMIMessageGeneric::Process(buffer, size);
	if (offset == 0)
		return 0;

	const ACEMIPacketData* message = (const ACEMIPacketData*)((const uint8*)buffer + offset);
	SetFrameFormat((ACEMIFrameFormat)message->m_control.m_frameFormat);
	SetRepeatFlag(message->m_control.m_repeatFlag);
	SetBroadcastType((ACEMIBroadcastType)message->m_control.m_systemBroadcast);
	SetPriority((ACEMIPriority)message->m_control.m_priority);
	SetAcknowledgeRequestFlag(message->m_control.m_acknowledgeRequestFlag);
	SetConfirmFlag(message->m_control.m_confirmFlag);
	SetAddressType((AAddressType)message->m_control.m_destinationAddresType);
	SetHopCount(message->m_control.m_hopCount);
	SetExtendedFrameFormat(message->m_control.m_extendedFrameFormat);
	SetTPCI((message->m_tapci0 & 0xFC) >> 2);
	SetSource(message->m_source);

	if (GetAPCI() == ACEMIAPCI::GroupValueRead || GetAPCI() == ACEMIAPCI::GroupValueResponse || GetAPCI() == ACEMIAPCI::GroupValueWrite)
		SetAddressType(AAddressType::GroupAddress);
	else
		SetAddressType(AAddressType::IndividualAddress);

	if (GetAddressType() == AAddressType::IndividualAddress)
		SetDestinationIndividual(message->m_destination);
	else
		SetDestinationGroup(message->m_destination);
	offset += sizeof(ACEMIPacketData);

	uint16 rawACPI = (uint16)((message->m_tapci0 & 0x03) << 8) | message->m_tapci1;
	ACEMIAPCI trimmedACPI = (ACEMIAPCI)(rawACPI & 0x03C0);
	if (trimmedACPI == ACEMIAPCI::GroupValueRead || trimmedACPI == ACEMIAPCI::GroupValueWrite || trimmedACPI == ACEMIAPCI::GroupValueResponse)
	{
		SetAPCI(trimmedACPI);
		SetFirstPayload((uint16)rawACPI & 0x003F);
	}
	else
	{
		SetAPCI((ACEMIAPCI)rawACPI);
		SetFirstPayload(0);
	}

	if (message->m_length != 0)
	{
		SetPayload((const uint8*)buffer + offset, message->m_length - 1);
		offset += message->m_length - 1;
	}
	else
	{
		SetPayload(nullptr, 0);
	}

	return offset;
}

void ACEMIMessageData::Reset()
{
	m_frameFormat = ACEMIFrameFormat::Standard;
	m_repeatFlag = false;
	m_systemBroadcast = ACEMIBroadcastType::Broadcast;
	m_priority = ACEMIPriority::Normal;
	m_acknowledgeRequestFlag = false;
	m_confirmFlag = false;
	m_destAddressType = AAddressType::GroupAddress;
	m_hopCount = 0;
	m_extendedFrameFormat = 0;
	m_firstPayload = 0;
	m_payloadSize = 0;
	m_tpci = 0x00;
	m_apci = ACEMIAPCI::GroupValueWrite;
	memset(m_payload, 0, sizeof(m_payload));
}

std::string ACEMIMessageData::ToString() const
{
	stringstream output;
	char hexOutput[5];

	output << ACEMIMessageGeneric::ToString();

	output << "  FrameFormat: " << (GetFrameFormat() == ACEMIFrameFormat::Standard ? "Standard" : "Extended") << "\n";
	output << "  Repeat: " << (GetRepeatFlag() ? "True" : "False") << "\n";
	output << "  System Broadcast: " << (GetBroadcastType() == ACEMIBroadcastType::Broadcast ? "Broadcast" : "System Broadcast") << "\n";

	output << "  Priority: ";
	switch (GetPriority())
	{
		case ACEMIPriority::System:
			output << "System";
			break;

		case ACEMIPriority::High:
			output << "High";
			break;

		case ACEMIPriority::Alarm:
			output << "Alarm";
			break;

		case ACEMIPriority::Normal:
			output << "Normal";
			break;

		default:
			output << "UNKNOWN";
			break;
	};
	output << "\n";

	output << "  Acknowledge Request Flag: " << (GetAcknowledgeRequestFlag() ? "True" : "False") << "\n";
	output << "  Confirm Flag: " << (GetConfirmFlag() == 1 ? "Error" : "No Error") << "\n";
	output << "  Address Type : " << (GetAddressType() == AAddressType::IndividualAddress ? "Individual" : "Group") << "\n";
	output << "  Hop Count: " << std::to_string(GetHopCount()) << "\n";
	output << "  Extended Frame Format: " << std::to_string(GetExtendedFrameFormat()) << "\n";
	output << "  Source : " << m_source.GetString() << "\n";
	if (GetAddressType() == AAddressType::IndividualAddress)
		output << "  Destination : " << m_destinationIndividual.GetString() << "\n";
	else
		output << "  Destination : " << m_destinationGroup.GetString() << "\n";

	toHex(m_tpci, hexOutput);
	output << "  TPCI: " << hexOutput << "\n";

	output << "  APCI: ";
	switch (m_apci)
	{
		case ACEMIAPCI::GroupValueRead:
			output << "GroupValueRead";
			break;

		case ACEMIAPCI::GroupValueResponse:
			output << "GroupValueResponse";
			break;

		case ACEMIAPCI::GroupValueWrite:
			output << "GroupValueWrite";
			break;

		case ACEMIAPCI::IndividualAddressWrite:
			output << "IndividualAddressWrite";
			break;

		case ACEMIAPCI::IndividualAddressRead:
			output << "IndividualAddressRead";
			break;

		case ACEMIAPCI::IndividualAddressResponse:
			output << "IndividualAddressResponse";
			break;

		case ACEMIAPCI::IndividualAddressSerialNumberRead:
			output << "IndividualAddressSerialNumberRead";
			break;

		case ACEMIAPCI::IndividualAddressSerialNumberResponse:
			output << "IndividualAddressSerialNumberResponse";
			break;

		case ACEMIAPCI::IndividualAddressSerialNumberWrite:
			output << "IndividualAddressSerialNumberWrite";
			break;

		case ACEMIAPCI::SystemNetworkParameterRead:
			output << "SystemNetworkParameterRead";
			break;

		case ACEMIAPCI::SystemNetworkParameterResponse:
			output << "SystemNetworkParameterResponse";
			break;

		case ACEMIAPCI::SystemNetworkParameterWrite:
			output << "SystemNetworkParameterWrite";
			break;

		case ACEMIAPCI::DomainAddressSerialNumberRead:
			output << "DomainAddressSerialNumberRead";
			break;

		case ACEMIAPCI::DomainAddressSerialNumberResponse:
			output << "DomainAddressSerialNumberResponse";
			break;

		case ACEMIAPCI::DomainAddressSerialNumberWrite:
			output << "DomainAddressSerialNumberWrite";
			break;

		case ACEMIAPCI::MemoryRead:
			output << "MemoryRead";
			break;

		case ACEMIAPCI::MemoryResponse:
			output << "MemoryResponse";
			break;

		case ACEMIAPCI::MemoryWrite:
			output << "MemoryWrite";
			break;

		case ACEMIAPCI::UserMemoryRead:
			output << "UserMemoryRead";
			break;

		case ACEMIAPCI::UserMemoryResponse:
			output << "UserMemoryResponse";
			break;

		case ACEMIAPCI::UserMemoryWrite:
			output << "UserMemoryWrite";
			break;

		case ACEMIAPCI::UserManufacturerInfoRead:
			output << "UserManufacturerInfoRead";
			break;

		case ACEMIAPCI::UserManufacturerInfoResponse:
			output << "";
			break;

		case ACEMIAPCI::DeviceDescriptorRead:
			output << "DeviceDescriptorRead";
			break;

		case ACEMIAPCI::DeviceDescriptorResponse:
			output << "DeviceDescriptorResponse";
			break;

		case ACEMIAPCI::Restart:
			output << "Restart";
			break;

		case ACEMIAPCI::AuthorizeRequest:
			output << "AuthorizeRequest";
			break;

		case ACEMIAPCI::AuthorizeResponse:
			output << "AuthorizeResponse";
			break;

		case ACEMIAPCI::KeyWrite:
			output << "KeyWrite";
			break;

		case ACEMIAPCI::KeyResponse:
			output << "KeyResponse";
			break;

		case ACEMIAPCI::PropertyValueRead:
			output << "PropertyValueRead";
			break;

		case ACEMIAPCI::PropertyValueResponse:
			output << "PropertyValueResponse";
			break;

		case ACEMIAPCI::PropertyValueWrite:
			output << "PropertyValueWrite";
			break;

		case ACEMIAPCI::PropertyDescriptionRead:
			output << "PropertyDescriptionRead";
			break;

		case ACEMIAPCI::PropertyDescriptionResponse:
			output << "PropertyDescriptionResponse";
			break;

		default:
			output << "UNKNOWN";
			break;
	}

	toHex16((uint16)m_apci, hexOutput);
	output << " (0x" << hexOutput << ")\n";

	toHex(GetFirstPayload(), hexOutput);
	output << "  First Payload: 0x" << hexOutput << "\n";
	output << "  Payload Size: " << std::to_string((uint8)GetPayloadSize()) << "\n";
	if (m_payloadSize != 0)
	{
		output << "  Payload : ";
		output << std::hex;
		for (size_t i = 0; i < m_payloadSize; i++)
		{
			char hexOutput[3];
			toHex(m_payload[i], hexOutput);
			output << hexOutput << " ";
		}
		output << "\n";
	}
	return output.str();
}

ACEMIMessageData::ACEMIMessageData()
{
	Reset();
}


//  ACEMIMessageProperty
////////////////////////////////////////////////////////////////////////////////////

 void ACEMIMessageProperty::SetInterfaceObjectType(uint16 type)
{
	 interfaceObjectType = type;
}

uint16 ACEMIMessageProperty::GetInterfaceObjectType() const
{
	return interfaceObjectType;
}

void ACEMIMessageProperty::SetObjectInstance(uint8 instance)
{
	objectInstance = instance;
}

uint8 ACEMIMessageProperty::GetObjectInstance() const
{
	return objectInstance;
}

 void ACEMIMessageProperty::SetPropertyId(uint8 id)
{
	 propertyId = id;
}

uint8 ACEMIMessageProperty::GetPropertyId() const
{
	return propertyId;
}

void ACEMIMessageProperty::SetArraySize(uint16 size)
{
	CheckError(size > sizeof(array) * 8, RETURN_VOID, "Array size too big.");
	arraySize = size;
}

uint8 ACEMIMessageProperty::GetArraySize() const
{
	return arraySize;
}

void ACEMIMessageProperty::SetArrayStartIndex(uint16 index)
{
	arrayStartIndex = index;
}

uint16 ACEMIMessageProperty::GetArrayStartIndex() const
{
	return arrayStartIndex;
}


void ACEMIMessageProperty::SetArrayElementSize(uint8 size)
{
	arrayElementSize = size;
}

uint8 ACEMIMessageProperty::GetArrayElementSize() const
{
	return arrayElementSize;
}

void ACEMIMessageProperty::SetArrayElement(uint8 offset, void* element)
{
	CheckError(offset * GetArrayElementSize() >= GetArraySize() * GetArrayElementSize(), RETURN_VOID, "Offset is too big.");
	memcpy(array + offset * GetArrayElementSize(), element, GetArrayElementSize());
}

const void* ACEMIMessageProperty::GetArrayElement(uint8 offset) const
{
	CheckError(offset * GetArrayElementSize() >= GetArraySize() * GetArrayElementSize(), nullptr, "Offset is too big.");
	return array + offset * GetArrayElementSize();
}

void ACEMIMessageProperty::SetRequestArraySize(bool request)
{
	requestArraySize = request;
}

bool ACEMIMessageProperty::GetRequestArraySize() const
{
	return requestArraySize;
}

void ACEMIMessageProperty::Generate(void* buffer, uint8& size) const
{
	ACEMIMessage::Generate(buffer, size);

	ACEMIPacketProperty* packet = (ACEMIPacketProperty*)((uint8*)buffer + size);
	packet->m_interfaceObjectType = bswap_16(GetInterfaceObjectType());
	packet->m_objectInstance = GetObjectInstance();
	packet->m_propertyId = GetPropertyId();

	if (requestArraySize)
	{
		packet->m_arraySize = 1;
		packet->m_arrayIndexH = 0;
		packet->m_arrayIndexL = 0;
	}
	else
	{
		packet->m_arraySize = GetArraySize();
		packet->m_arrayIndexH = GetArrayStartIndex() >> 8;
		packet->m_arrayIndexL = GetArraySize() & 0xFF;

		if (GetMessageCode() == ACEMIMessageCode::PropertyReadConfirmation ||
			GetMessageCode() == ACEMIMessageCode::PropertyWriteRequest)
		{
			memcpy((uint8*)buffer + size, array, GetArraySize() * GetArrayElementSize());
			size += GetArraySize() * GetArrayElementSize();
		}
	}

	size += (uint8)sizeof(ACEMIPacketProperty);
}

size_t ACEMIMessageProperty::Process(const void* buffer, size_t size)
{
	size_t offset = ACEMIMessage::Process(buffer, size);

	ACEMIPacketProperty* packet = (ACEMIPacketProperty*)((uint8*)buffer + offset);
	SetInterfaceObjectType(bswap_16(packet->m_interfaceObjectType));
	SetObjectInstance(packet->m_objectInstance);
	SetPropertyId(packet->m_propertyId);
	SetArraySize(packet->m_arraySize);
	SetArrayStartIndex(packet->m_arrayIndexH << 8 | packet->m_arrayIndexL);
	offset += sizeof(ACEMIPacketProperty);

	if (GetArraySize() == 0)
	{
		errorCode = *(uint8*)buffer + offset;
	}
	else if (GetArraySize() == 1 && GetArrayStartIndex() == 0)
	{
		SetRequestArraySize(true);
		arraySize = GetArraySize() * GetArrayElementSize();
	}
	else
	{
		if (GetMessageCode() == ACEMIMessageCode::PropertyReadConfirmation ||
			GetMessageCode() == ACEMIMessageCode::PropertyWriteRequest)
		{
			size_t arraySize = GetArraySize() * GetArrayElementSize();
			memcpy(array, (uint8*)buffer + offset, arraySize);
			offset += arraySize;
		}
	}

	return offset;
}

std::string ACEMIMessageProperty::ToString() const
{
	stringstream output;

	output << ACEMIMessage::ToString();

	output << "  <Property>" << "\n";
	output << "  Interface Object Type: " << std::to_string(GetInterfaceObjectType()) << "\n";
	output << "  Object Instance: " << std::to_string(GetObjectInstance()) << "\n";
	output << "  Property ID: " << std::to_string(GetPropertyId()) << "\n";
	output << "  Array Size: " << std::to_string(GetArraySize()) << "\n";
	output << "  Array Index: " << std::to_string(GetArrayStartIndex()) << "\n";
	output << "  Array Element Size: " << std::to_string(GetArrayElementSize()) << "\n";


	if (GetArraySize() == 0)
	{
		output << "  Error Code:" << errorCode << "\n";
	}
	else if (GetArraySize() == 1 && GetArrayStartIndex() == 0)
	{
		output << "  Array Size Request: " << (requestArraySize ? "True" : "False") << "\n";
	}
	else
	{
		if (GetMessageCode() == ACEMIMessageCode::PropertyReadConfirmation ||
			GetMessageCode() == ACEMIMessageCode::PropertyWriteRequest)
		{
			output << "  Array: " << "\n";

			for (auto i = 0; i < GetArraySize(); i++)
			{
				uint8* value = (uint8*)GetArrayElement(i);
				output << "    [" << std::to_string(i) << "] = ";
				for (size_t n = 0; n < GetArrayElementSize(); n++)
				{
					char hexOutput[3];
					toHex(value[n], hexOutput);
					output << hexOutput << " ";
				}
				output << "\n";
			}
		}
	}

	return output.str();
}

ACEMIMessageProperty::ACEMIMessageProperty()
{
	interfaceObjectType = 0;
	objectInstance = 1;
	propertyId = 0;
	arrayElementSize = 1;
	arraySize = 0;
	arrayStartIndex = 0;
	memset(array, 0, sizeof(array));
	requestArraySize = false;
	errorCode = 0;
}
