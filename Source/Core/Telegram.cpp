#include "Telegram.h"

#include "TelegramPacket.h"
#include "Common/Error.h"

void OHTelegram::SetPriority(OHTelegramPriority priority)
{
	this->priority = priority;
}

OHTelegramPriority OHTelegram::GetPriority() const
{
	return priority;
}

void OHTelegram::SetAddressType(OHTelegramAddressType type)
{
	addressType = type;
}

OHTelegramAddressType OHTelegram::GetAddressType() const
{
	return addressType;
}

void OHTelegram::SetSource(const OHAddress& address)
{
	source = address;
}

const OHAddress& OHTelegram::GetSource() const
{
	return source;
}

void OHTelegram::SetDestination(const OHAddress& address)
{
	destination = address;
}

const OHAddress& OHTelegram::GetDestination() const
{
	return destination;
}

void OHTelegram::SetRepeatFlag(bool flag)
{
	repeatFlag = flag;
}

bool OHTelegram::GetRepeatFlag() const
{
	return repeatFlag;
}

void OHTelegram::SetCommand(OHTelegramCommand command)
{
	this->command = command;
}

OHTelegramCommand OHTelegram::GetCommand() const
{
	return command;
}

void OHTelegram::SetFirstPayload(uint8 value)
{
	firstPayload = value;
}

uint8 OHTelegram::GetFirstPayload() const
{
	return firstPayload;
}

void OHTelegram::SetPayload(void* data, size_t size)
{
	CheckError(size > 13, RETURN_VOID, "Cannot set payload. Payload is too big.");

	payload.resize(size);
	memcpy(payload.begin()._Ptr, data, size);
}

void* OHTelegram::GetPayload() const
{
	return payload.begin()._Ptr;
}

size_t OHTelegram::GetPayloadSize() const
{
	return payload.size();
}

void OHTelegram::Generate(void* buffer, size_t& size) const
{
	OHTelegramPacket* packet = (OHTelegramPacket*)buffer;
	packet->control.frameFormat = (uint8)OHTelegramFrameFormat::Standard;
	packet->control.reserved0 = 0x01;
	packet->control.repeadFlag = repeatFlag;
	packet->control.reserved1 = 0x00;
	packet->source = source.GetRaw();
	packet->destination = source.GetRaw();
	packet->routing.counter = 0x00;
	packet->routing.destinationAddressType = (uint8)GetAddressType();
	packet->routing.payloadLenght = GetPayloadSize();
	
	uint8* bytes = (uint8*)buffer;
	if (payload.size() != 0)
		memcpy(bytes + TELEGRAM_PAYLOAD_OFFSET, payload.begin()._Ptr, payload.size());

	size = TELEGRAM_PAYLOAD_OFFSET + GetPayloadSize();
	uint8 checkSum = 0;
	for (uint8 I = 0; I < size; I++)
		checkSum ^= bytes[I];
	bytes[size] = ~checkSum;
	
	size++;
}

bool OHTelegram::Process(const void* buffer, size_t size)
{
	CheckError(size < TELEGRAM_MIN_SIZE, false, "Process telegram failed. Telegram size is too small.");
		
	OHTelegramPacket* packet = (OHTelegramPacket*)buffer;
	uint8 packetSize = packet->routing.payloadLenght + TELEGRAM_MIN_SIZE;
	CheckError(packetSize != size, false, "Process telegram failed. Telegram payload size is wrong.");
	CheckError(packet->control.frameFormat != (uint8)OHTelegramFrameFormat::Standard, false, "Process telegram failed. Only standard format telegrams are supported.");
	CheckWarning(packet->control.reserved0 != true || packet->control.reserved1 != 0x00, "Control field unused fields default values are incorrect.");

	uint8* bytes = (uint8*)buffer;
	uint8 checksum = 0;
	for (uint8 I = 0; I < packetSize; I++)
		checksum ^= bytes[I];
	CheckError(checksum != 0xFF, false, "Process telegram failed. Checksum incorrect.");
	
	SetRepeatFlag(packet->control.repeadFlag);
	SetSource(packet->source);
	SetDestination(packet->source);
	SetCommand((OHTelegramCommand)packet->command.command);
	SetFirstPayload(packet->command.payload);
	SetAddressType((OHTelegramAddressType)packet->routing.destinationAddressType);
	SetPayload(bytes + TELEGRAM_PAYLOAD_OFFSET, packet->routing.payloadLenght);

	return true;
}

OHTelegram::OHTelegram()
{
	repeatFlag = false;
	priority = OHTelegramPriority::Normal;
	addressType = OHTelegramAddressType::GroupAddress;
	command = OHTelegramCommand::ValueWrite;
	firstPayload = 0;
}