#include "ACEMIMessage.h"

#include "ACEMIProtocol.h"
#include "ACommon/AError.h"


#include <sstream>

//  ACEMIMessage
////////////////////////////////////////////////////////////////////////////////////

void ACEMIMessage::SetIndex(size_t index)
{
	this->index = index;
}

size_t ACEMIMessage::GetIndex() const
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

void ACEMIMessage::SetAdditionalInfo(const void* data, uint8 size)
{
	if (size != 0)
		memcpy(additionalInfo, data, size);
	additionalInfoSize = size;
}

const void* ACEMIMessage::GetAdditionalInfo() const
{
	return additionalInfo;
}

uint8 ACEMIMessage::GetAdditionalInfoSize() const
{
	return additionalInfoSize;
}

void ACEMIMessage::Generate(void* buffer, uint8& size) const
{
	ACEMIHeader* header = (ACEMIHeader*)buffer;
	header->messageCode = (uint8)GetMessageCode();
	header->additionalInfoSize = GetAdditionalInfoSize();
	if (header->additionalInfoSize != 0)
		memcpy((uint8*)buffer + sizeof(header), GetAdditionalInfo(), GetAdditionalInfoSize());
}

size_t ACEMIMessage::Process(const void* buffer, size_t size)
{
	ACEMIHeader* header = (ACEMIHeader*)buffer;
	SetMessageCode((ACEMIMessageCode)header->messageCode);
	SetAdditionalInfo((uint8*)buffer + sizeof(ACEMIHeader), header->additionalInfoSize);

	return sizeof(ACEMIHeader) + header->additionalInfoSize;
}

std::string ACEMIMessage::Print() const
{
	stringstream output;
	output << "Telegram #" << index << "\n";
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

ACEMIMessage::ACEMIMessage()
{
	index = 0;
	messageCode = ACEMIMessageCode::Data_Transmit;
	memset(additionalInfo, 0, sizeof(additionalInfo));
	additionalInfoSize = 0;
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
	memset(payload + size, 0, sizeof(payload) - size);
	memcpy(payload, data, size);
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
	*(uint16*)payload = value;
	payloadSize = 2;
}

uint16 ACEMIMessageData::GetUInt16() const
{
	return *(int16*)payload;
}

void ACEMIMessageData::SetUInt32(uint32 value)
{
	*(uint32*)payload = value;
	payloadSize = 4;
}

uint32 ACEMIMessageData::GetUInt32() const
{
	return *(uint32*)payload;
}

void ACEMIMessageData::SetUInt64(uint64 value)
{
	*(uint64*)payload = value;
	payloadSize = 8;
}

uint64 ACEMIMessageData::GetUInt64() const
{
	return *(uint64*)payload;
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
	*(int16*)payload = value;
	payloadSize = 2;
}

int16 ACEMIMessageData::GetInt16() const
{
	return *(int16*)payload;
}

void ACEMIMessageData::SetInt32(int32 value)
{
	*(int32*)payload = value;
	payloadSize = 4;
}

int32 ACEMIMessageData::GetInt32() const
{
	return *(int32*)payload;
}

void ACEMIMessageData::SetInt64(int64 value)
{
	*(int64*)payload = value;
	payloadSize = 8;
}

int64 ACEMIMessageData::GetInt64() const
{
	return *(int64*)payload;
}

void ACEMIMessageData::SetHalf(float value)
{
	*(float*)payload = value;
	payloadSize = 2;
}

float ACEMIMessageData::GetHalf() const
{
	return *(float*)payload;
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
	ACEMIMessage::Generate(buffer, size);

	ACEMIDataPacket* packet = (ACEMIDataPacket*)buffer;
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

	packet->length = payloadSize;
	packet->tapci0 = tpci << 2 | ((uint16)apci & 0x0300);
	packet->tapci1 = ((uint16)apci & 0x00FF);
	


	ACEMIAPCI trimmedACPI = GetAPCI();
	if (trimmedACPI == ACEMIAPCI::GroupValueRead || trimmedACPI == ACEMIAPCI::GroupValueWrite || trimmedACPI == ACEMIAPCI::GroupValueResponse)
		packet->tapci1 = packet->tapci1 | GetFirstPayload();

	uint8* bytes = (uint8*)buffer;
	if (payloadSize != 0)
		memcpy(bytes + sizeof(ACEMIDataPacket), payload, payloadSize);

	/*size = sizeof(ACEMIDataPacket) + GetPayloadSize();
	uint8 checkSum = 0;
	for (uint8 I = 0; I < size; I++)
		checkSum ^= bytes[I];
	bytes[size] = ~checkSum;
	
	size++;*/
}

size_t ACEMIMessageData::Process(const void* buffer, size_t size)
{
	size_t offset = ACEMIMessage::Process(buffer, size);
	if (offset == 0)
		return 0;

	const ACEMIDataPacket* message = (const ACEMIDataPacket*)((const uint8*)buffer + offset);
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
	if (GetAddressType() == AAddressType::IndividualAddress)
		SetDestinationIndividual(message->destination);
	else
		SetDestinationGroup(message->destination);
	offset += sizeof(ACEMIDataPacket);

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

	SetPayload((const uint8*)buffer + offset, message->length - 1);
	offset += message->length - 1;

	return offset;
}

std::string ACEMIMessageData::Print() const
{
	stringstream output;
	char hexOutput[5];

	output << ACEMIMessage::Print();

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
	output << "  Confirm Flag: " << (GetConfirmFlag() ? "True" : "False") << "\n";
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
	if (payloadSize == 0)
	{
		char hexOutput[3];
		toHex(payloadSize, hexOutput);
		output << "  First Payload : " << hexOutput << "\n";
	}
	else
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