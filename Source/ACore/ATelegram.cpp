#include "ATelegram.h"

#include "ATelegramPacket.h"
#include "ACommon/AError.h"

void ATelegram::SetPriority(ATelegramPriority priority)
{
	this->priority = priority;
}

ATelegramPriority ATelegram::GetPriority() const
{
	return priority;
}

void ATelegram::SetAddressType(ATelegramAddressType type)
{
	addressType = type;
}

ATelegramAddressType ATelegram::GetAddressType() const
{
	return addressType;
}

void ATelegram::SetSource(const AAddress& address)
{
	source = address;
}

const AAddress& ATelegram::GetSource() const
{
	return source;
}

void ATelegram::SetDestination(const AAddress& address)
{
	destination = address;
}

const AAddress& ATelegram::GetDestination() const
{
	return destination;
}

void ATelegram::SetRepeatFlag(bool flag)
{
	repeatFlag = flag;
}

bool ATelegram::GetRepeatFlag() const
{
	return repeatFlag;
}

void ATelegram::SetCommand(ATelegramCommand command)
{
	this->command = command;
}

ATelegramCommand ATelegram::GetCommand() const
{
	return command;
}

void ATelegram::SetFirstPayload(uint8 value)
{
	firstPayload = value;
}

uint8 ATelegram::GetFirstPayload() const
{
	return firstPayload;
}

void ATelegram::SetPayload(const void* data, size_t size)
{
	CheckError(size > A_TELEGRAM_MAX_PAYLOAD_SIZE, RETURN_VOID, "Cannot set payload. Payload is too big.");

	payload.resize(size);
	memcpy(payload.begin()._Ptr, data, size);
}

const void* ATelegram::GetPayload() const
{
	return payload.begin()._Ptr;
}

size_t ATelegram::GetPayloadSize() const
{
	return payload.size();
}

void ATelegram::Generate(void* buffer, size_t& size) const
{
	ATelegramPacket* packet = (ATelegramPacket*)buffer;
	packet->control.frameFormat = (uint8)ATelegramFrameFormat::Standard;
	packet->control.reserved0 = 0x01;
	packet->control.repeadFlag = GetRepeatFlag();
	packet->control.reserved1 = 0x00;
	packet->source = source.GetRaw();
	packet->destination = destination.GetRaw();
	packet->routing.counter = 0x00;
	packet->routing.destinationAddressType = (uint8)GetAddressType();
	packet->routing.payloadLenght = GetPayloadSize();
	packet->command.command = (uint8)GetCommand();
	packet->command.payload = GetFirstPayload();

	uint8* bytes = (uint8*)buffer;
	if (payload.size() != 0)
		memcpy(bytes + A_TELEGRAM_PAYLOAD_OFFSET, payload.begin()._Ptr, payload.size());

	size = A_TELEGRAM_PAYLOAD_OFFSET + GetPayloadSize();
	uint8 checkSum = 0;
	for (uint8 I = 0; I < size; I++)
		checkSum ^= bytes[I];
	bytes[size] = ~checkSum;
	
	size++;
}

bool ATelegram::Process(const void* buffer, size_t size)
{
	CheckError(size < A_TELEGRAM_MIN_SIZE, false, "Process telegram failed. Telegram size is too small.");
		
	ATelegramPacket* packet = (ATelegramPacket*)buffer;
	uint8 packetSize = packet->routing.payloadLenght + A_TELEGRAM_MIN_SIZE;
	CheckError(packetSize != size, false, "Process telegram failed. Telegram payload size is wrong.");
	CheckError(packet->control.frameFormat != (uint8)ATelegramFrameFormat::Standard, false, "Process telegram failed. Only standard format telegrams are supported.");
	CheckWarning(packet->control.reserved0 != true || packet->control.reserved1 != 0x00, "Control field unused fields default values are incorrect.");

	uint8* bytes = (uint8*)buffer;
	uint8 checksum = 0;
	for (uint8 I = 0; I < packetSize; I++)
		checksum ^= bytes[I];
	CheckError(checksum != 0xFF, false, "Process telegram failed. Checksum incorrect.");
	
	SetRepeatFlag(packet->control.repeadFlag);
	SetSource(packet->source);
	SetDestination(packet->destination);
	SetCommand((ATelegramCommand)packet->command.command);
	SetFirstPayload(packet->command.payload);
	SetAddressType((ATelegramAddressType)packet->routing.destinationAddressType);
	SetPayload(bytes + A_TELEGRAM_PAYLOAD_OFFSET, packet->routing.payloadLenght);

	return true;
}

ATelegram::ATelegram()
{
	repeatFlag = false;
	priority = ATelegramPriority::Normal;
	addressType = ATelegramAddressType::GroupAddress;
	command = ATelegramCommand::ValueWrite;
	firstPayload = 0;
}