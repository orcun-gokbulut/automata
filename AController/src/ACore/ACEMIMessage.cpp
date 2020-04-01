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
	this->index = index;
}

uint64 ACEMIMessage::GetIndex() const
{
	return index;
}

void ACEMIMessage::SetMessageCode(ACEMIMessageCode code)
{
	messageCode = code;
}

ACEMIMessageCode ACEMIMessage::GetMessageCode() const
{
	return messageCode;
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
	output << "cEMI Message #" << index << "\n";

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
	index = 0;
	messageCode = ACEMIMessageCode::DataTransmitRequest;
}

ACEMIMessage::~ACEMIMessage()
{

}


//  ACEMIMessageGeneric
////////////////////////////////////////////////////////////////////////////////////

void ACEMIMessageGeneric::SetAdditionalInfo(const void* data, uint8 size)
{
	if (size != 0)
		memcpy(additionalInfo, data, size);
	additionalInfoSize = size;
}

const void* ACEMIMessageGeneric::GetAdditionalInfo() const
{
	return additionalInfo;
}

uint8 ACEMIMessageGeneric::GetAdditionalInfoSize() const
{
	return additionalInfoSize;
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
	memset(additionalInfo, 0, sizeof(additionalInfo));
	additionalInfoSize = 0;
}

ACEMIMessageGeneric::~ACEMIMessageGeneric()
{

}


//  ACEMIMessageData
////////////////////////////////////////////////////////////////////////////////////

void ACEMIMessageData::SetFrameFormat(ACEMIFrameFormat format)
{
	frameFormat = format;
}

ACEMIFrameFormat ACEMIMessageData::GetFrameFormat() const
{
	return frameFormat;
}

void ACEMIMessageData::SetRepeatFlag(bool flag)
{
	repeatFlag = flag;
}

bool ACEMIMessageData::GetRepeatFlag() const
{
	return repeatFlag;
}

void ACEMIMessageData::SetBroadcastType(ACEMIBroadcastType type)
{
	systemBroadcast = type;
}

ACEMIBroadcastType ACEMIMessageData::GetBroadcastType() const
{
	return systemBroadcast;
}

void ACEMIMessageData::SetPriority(ACEMIPriority priority)
{
	this->priority = priority;
}

ACEMIPriority ACEMIMessageData::GetPriority() const
{
	return priority;
}

void ACEMIMessageData::SetAcknowledgeRequestFlag(bool flag)
{
	acknowledgeRequestFlag = flag;
}

bool ACEMIMessageData::GetAcknowledgeRequestFlag() const
{
	return acknowledgeRequestFlag;
}

void ACEMIMessageData::SetConfirmFlag(bool flag)
{
	confirmFlag = flag;
}

bool ACEMIMessageData::GetConfirmFlag() const
{
	return confirmFlag;
}

void ACEMIMessageData::SetHopCount(uint8 count)
{
	hopCount = count;
}

uint8 ACEMIMessageData::GetHopCount() const
{
	return hopCount;
}

void ACEMIMessageData::SetExtendedFrameFormat(uint8 format)
{
	extendedFrameFormat = format;
}

uint8 ACEMIMessageData::GetExtendedFrameFormat() const
{
	return extendedFrameFormat;
}

void ACEMIMessageData::SetAddressType(AAddressType type)
{
	destAddressType = type;
}

AAddressType ACEMIMessageData::GetAddressType() const
{
	return destAddressType;
}

void ACEMIMessageData::SetSource(const AIndividualAddress& address)
{
	source = address;
}

const AIndividualAddress& ACEMIMessageData::GetSource() const
{
	return source;
}

void ACEMIMessageData::SetDestinationIndividual(const AIndividualAddress& address)
{
	destinationIndividual = address;
}

const AIndividualAddress& ACEMIMessageData::GetDestinationInduvidual() const
{
	return destinationIndividual;
}

void ACEMIMessageData::SetDestinationGroup(const AGroupAddress& address)
{
	destinationGroup = address;
}

const AGroupAddress& ACEMIMessageData::GetDestinationGroup() const
{
	return destinationGroup;
}

void ACEMIMessageData::SetTPCI(uint8 value)
{
	tpci = value;
}

uint8 ACEMIMessageData::GetTPCI() const
{
	return tpci;
}

void ACEMIMessageData::SetAPCI(ACEMIAPCI value)
{
	apci = value;
}

ACEMIAPCI ACEMIMessageData::GetAPCI() const
{
	return apci;
}

void ACEMIMessageData::SetFirstPayload(uint8 payload)
{
	firstPayload = payload;
}

uint8 ACEMIMessageData::GetFirstPayload() const
{
	return firstPayload;
}

void ACEMIMessageData::SetPayload(const void* data, uint8 size)
{
	if (data == nullptr || size == 0)
	{
		memset(payload, 0, sizeof(payload));
	}
	else
	{
		memset(payload + size, 0, sizeof(payload) - size);
		memcpy(payload, data, size);
	}

	payloadSize = size;
}

const void* ACEMIMessageData::GetPayload() const
{
	return payload;
}

uint8 ACEMIMessageData::GetPayloadSize() const
{
	return payloadSize;
}

void ACEMIMessageData::SetUInt8(uint8 value)
{
	*(uint8*)payload = value;
	payloadSize = 1;
}

uint8 ACEMIMessageData::GetUInt8() const
{
	return *(int8*)payload;
}

void ACEMIMessageData::SetUInt16(uint16 value)
{
	*(uint16*)payload = bswap_16(value);
	payloadSize = 2;
}

uint16 ACEMIMessageData::GetUInt16() const
{
	return bswap_16(*(int16*)payload);
}

void ACEMIMessageData::SetUInt32(uint32 value)
{
	*(uint32*)payload = bswap_32(value);
	payloadSize = 4;
}

uint32 ACEMIMessageData::GetUInt32() const
{
	return bswap_32(*(uint32*)payload);
}

void ACEMIMessageData::SetUInt64(uint64 value)
{
	*(uint64*)payload = bswap_64(value);
	payloadSize = 8;
}

uint64 ACEMIMessageData::GetUInt64() const
{
	return bswap_64(*(uint64*)payload);
}

void ACEMIMessageData::SetInt8(int8 value)
{
	*(int8*)payload = value;
	payloadSize = 1;
}

int8 ACEMIMessageData::GetInt8() const
{
	return *(int8*)payload;
}

void ACEMIMessageData::SetInt16(int16 value)
{
	*(int16*)payload = (int16)bswap_16(value);
	payloadSize = 2;
}

int16 ACEMIMessageData::GetInt16() const
{
	return (int16)bswap_16(*(int16*)payload);
}

void ACEMIMessageData::SetInt32(int32 value)
{
	*(int32*)payload = (int32)bswap_32(value);
	payloadSize = 4;
}

int32 ACEMIMessageData::GetInt32() const
{
	return (int32)bswap_32(*(int32*)payload);
}

void ACEMIMessageData::SetInt64(int64 value)
{
	*(int64*)payload = (int64)bswap_64(value);
	payloadSize = 8;
}

int64 ACEMIMessageData::GetInt64() const
{
	return (int64)bswap_64(*(int64*)payload);
}

void ACEMIMessageData::SetHalfFloat(float value)
{
	*(uint16*)payload = toHalf(value);
	payloadSize = 2;
}

float ACEMIMessageData::GetHalfFloat() const
{
	return toFloat(*(uint16*)payload);
}

void ACEMIMessageData::SetFloat(float value)
{
	*(float*)payload = value;
	payloadSize = 4;
}

float ACEMIMessageData::GetFloat() const
{
	return *(float*)payload;
}

void ACEMIMessageData::SetDouble(double value)
{
	*(double*)payload = value;
	payloadSize = 8;
}

double ACEMIMessageData::GetDouble() const
{
	return *(double*)payload;
}

void ACEMIMessageData::SetBoolean(bool value)
{
	SetFirstPayload((uint8)value);
}

bool ACEMIMessageData::GetBoolean() const
{
	return (bool)firstPayload;
}

uint8 ACEMIMessageData::GetSize() const
{
	return GetPayloadSize() + A_TELEGRAM_MIN_SIZE;
}

void ACEMIMessageData::Generate(void* buffer, uint8& size) const
{
	ACEMIMessageGeneric::Generate(buffer, size);

	ACEMIPacketData* packet = (ACEMIPacketData*)((uint8*)buffer + size);
	packet->control.frameFormat = (uint8)GetFrameFormat();
	packet->control.reserved = 0;
	packet->control.repeatFlag = GetRepeatFlag();
	packet->control.systemBroadcast = (uint8)GetBroadcastType();
	packet->control.priority = (uint8)GetPriority();
	packet->control.acknowledgeRequestFlag = GetAcknowledgeRequestFlag();
	packet->control.confirmFlag = GetConfirmFlag();
	packet->control.destinationAddresType = (uint8)GetAddressType();
	packet->control.hopCount = GetHopCount();
	packet->control.extendedFrameFormat = GetExtendedFrameFormat();
	packet->source = source.GetRaw();
	if (GetAddressType() == AAddressType::IndividualAddress)
		packet->destination = destinationIndividual.GetRaw();
	else
		packet->destination = destinationGroup.GetRaw();

	packet->length = payloadSize + 1;
	packet->tapci0 = tpci << 2 | ((uint16)apci & 0x0300);
	packet->tapci1 = ((uint16)apci & 0x00FF);
	
	ACEMIAPCI trimmedACPI = GetAPCI();
	if (trimmedACPI == ACEMIAPCI::GroupValueRead || trimmedACPI == ACEMIAPCI::GroupValueWrite || trimmedACPI == ACEMIAPCI::GroupValueResponse)
		packet->tapci1 = packet->tapci1 | GetFirstPayload();

	size += sizeof(ACEMIPacketData);

	uint8* bytes = (uint8*)buffer;
	if (payloadSize != 0)
		memcpy(bytes + size, payload, payloadSize);

	size += payloadSize;
}

size_t ACEMIMessageData::Process(const void* buffer, size_t size)
{
	size_t offset = ACEMIMessageGeneric::Process(buffer, size);
	if (offset == 0)
		return 0;

	const ACEMIPacketData* message = (const ACEMIPacketData*)((const uint8*)buffer + offset);
	SetFrameFormat((ACEMIFrameFormat)message->control.frameFormat);
	SetRepeatFlag(message->control.repeatFlag);
	SetBroadcastType((ACEMIBroadcastType)message->control.systemBroadcast);
	SetPriority((ACEMIPriority)message->control.priority);
	SetAcknowledgeRequestFlag(message->control.acknowledgeRequestFlag);
	SetConfirmFlag(message->control.confirmFlag);
	SetAddressType((AAddressType)message->control.destinationAddresType);
	SetHopCount(message->control.hopCount);
	SetExtendedFrameFormat(message->control.extendedFrameFormat);
	SetTPCI((message->tapci0 & 0xFC) >> 2);
	SetSource(message->source);

	if (GetAPCI() == ACEMIAPCI::GroupValueRead || GetAPCI() == ACEMIAPCI::GroupValueResponse || GetAPCI() == ACEMIAPCI::GroupValueWrite)
		SetAddressType(AAddressType::GroupAddress);
	else
		SetAddressType(AAddressType::IndividualAddress);

	if (GetAddressType() == AAddressType::IndividualAddress)
		SetDestinationIndividual(message->destination);
	else
		SetDestinationGroup(message->destination);
	offset += sizeof(ACEMIPacketData);

	uint16 rawACPI = (uint16)((message->tapci0 & 0x03) << 8) | message->tapci1;
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

	if (message->length != 0)
	{
		SetPayload((const uint8*)buffer + offset, message->length - 1);
		offset += message->length - 1;
	}
	else
	{
		SetPayload(nullptr, 0);
	}

	return offset;
}

void ACEMIMessageData::Reset()
{
	frameFormat = ACEMIFrameFormat::Standard;
	repeatFlag = false;
	systemBroadcast = ACEMIBroadcastType::Broadcast;
	priority = ACEMIPriority::Normal;
	acknowledgeRequestFlag = false;
	confirmFlag = false;
	destAddressType = AAddressType::GroupAddress;
	hopCount = 0;
	extendedFrameFormat = 0;
	firstPayload = 0;
	payloadSize = 0;
	tpci = 0x00;
	apci = ACEMIAPCI::GroupValueWrite;
	memset(payload, 0, sizeof(payload));
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
	output << "  Source : " << source.GetString() << "\n";
	if (GetAddressType() == AAddressType::IndividualAddress)
		output << "  Destination : " << destinationIndividual.GetString() << "\n";
	else
		output << "  Destination : " << destinationGroup.GetString() << "\n";

	toHex(tpci, hexOutput);
	output << "  TPCI: " << hexOutput << "\n";

	output << "  APCI: ";
	switch (apci)
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
	
	toHex16((uint16)apci, hexOutput);
	output << " (0x" << hexOutput << ")\n";

	toHex(GetFirstPayload(), hexOutput);
	output << "  First Payload: 0x" << hexOutput << "\n";
	output << "  Payload Size: " << std::to_string((uint8)GetPayloadSize()) << "\n";
	if (payloadSize != 0)
	{
		output << "  Payload : ";
		output << std::hex;
		for (size_t i = 0; i < payloadSize; i++)
		{
			char hexOutput[3];
			toHex(payload[i], hexOutput);
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
	packet->interfaceObjectType = bswap_16(GetInterfaceObjectType());
	packet->objectInstance = GetObjectInstance();
	packet->propertyId = GetPropertyId();

	if (requestArraySize)
	{
		packet->arraySize = 1;
		packet->arrayIndexH = 0;
		packet->arrayIndexL = 0;
	}
	else
	{
		packet->arraySize = GetArraySize();
		packet->arrayIndexH = GetArrayStartIndex() >> 8;
		packet->arrayIndexL = GetArraySize() & 0xFF;

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
	SetInterfaceObjectType(bswap_16(packet->interfaceObjectType));
	SetObjectInstance(packet->objectInstance);
	SetPropertyId(packet->propertyId);
	SetArraySize(packet->arraySize);
	SetArrayStartIndex(packet->arrayIndexH << 8 | packet->arrayIndexL);
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
