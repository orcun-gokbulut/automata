#pragma once

#include "AAddress.h"
#include "ACommon/ATypes.h"

#include <string>
#include <vector>
//#include <half.hpp>

//using half_float::half;

enum class ACEMIMessageCode : uint8
{
	Data_Received = 29,
	Data_Transmit = 11,
	Poll_Received = 9,
	Poll_Transmit = 3
};

enum class ACEMIFrameFormat : uint8
{
	Extended = 0,
	Standard = 1,
};

enum class ACEMIBroadcastType
{
	SystemBroadcast = 0,
	Broadcast = 1
};

enum class ACEMIPriority : uint8
{
	System = 0,
	High = 1,
	Alarm = 2,
	Normal = 3
};

enum class ACEMIDestAddressType
{
	Individual = 0,
	Group = 1
};

enum class ACEMIAPCI
{
	GroupValueRead = 0x0000,
	GroupValueResponse = 0x0040,
	GroupValueWrite = 0x0080,
	IndividualAddressWrite = 0x00c0,
	IndividualAddressRead = 0x0100,
	IndividualAddressResponse = 0x0140,
	IndividualAddressSerialNumberRead = 0x03dc,
	IndividualAddressSerialNumberResponse = 0x03dd,
	IndividualAddressSerialNumberWrite = 0x03de,
	SystemNetworkParameterRead = 0x01c8,
	SystemNetworkParameterResponse = 0x01c9,
	SystemNetworkParameterWrite = 0x01ca,
	DomainAddressSerialNumberRead = 0x03ec,
	DomainAddressSerialNumberResponse = 0x03ed,
	DomainAddressSerialNumberWrite = 0x03ee,
	MemoryRead = 0x0200,
	MemoryResponse = 0x0240,
	MemoryWrite = 0x0280,
	UserMemoryRead = 0x02C0,
	UserMemoryResponse = 0x02C1,
	UserMemoryWrite = 0x02C2,
	UserManufacturerInfoRead = 0x02C5,
	UserManufacturerInfoResponse = 0x2C6,
	DeviceDescriptorRead = 0x0300,
	DeviceDescriptorResponse = 0x0340,
	Restart = 0x0380,
	AuthorizeRequest = 0x03d1,
	AuthorizeResponse = 0x03d2,
	KeyWrite = 0x03d3,
	KeyResponse = 0x03d4,
	PropertyValueRead = 0x03d5,
	PropertyValueResponse = 0x03d6,
	PropertyValueWrite = 0x03d7,
	PropertyDescriptionRead = 0x03d8,
	PropertyDescriptionResponse = 0x03d9
};

class ACEMIMessage
{
	private:
		size_t						index;

		ACEMIMessageCode			messageCode;
		uint8						additionalInfo[256];
		uint8						additionalInfoSize;

	public:
		void						SetIndex(size_t Index);
		size_t						GetIndex() const;

		void						SetMessageCode(ACEMIMessageCode code);
		ACEMIMessageCode			GetMessageCode() const;

		void						SetAdditionalInfo(const void* data, uint8 size);
		const void*					GetAdditionalInfo() const;
		uint8						GetAdditionalInfoSize() const;

		void						Generate(void* buffer, uint8& size) const;
		size_t						Process(const void* buffer, size_t size);

		std::string					Print() const;

									ACEMIMessage();
};

class ACEMIMessageData : public ACEMIMessage
{
	private:
		ACEMIFrameFormat			frameFormat;
		bool						repeatFlag;
		ACEMIBroadcastType			systemBroadcast;
		ACEMIPriority				priority;
		bool						acknowledgeRequestFlag;
		bool						confirmFlag;
		AAddressType				destAddressType;
		uint8						hopCount;
		uint8						extendedFrameFormat;

		AIndividualAddress			source;
		AGroupAddress				destinationGroup;
		AIndividualAddress			destinationIndividual;

		uint8						tpci;
		ACEMIAPCI					apci;

		uint8						firstPayload;
		uint8						payload[256];
		uint8						payloadSize;

	public:
		void						SetFrameFormat(ACEMIFrameFormat format);
		ACEMIFrameFormat			GetFrameFormat() const;

		void						SetRepeatFlag(bool type);
		bool						GetRepeatFlag() const;

		void						SetBroadcastType(ACEMIBroadcastType type);
		ACEMIBroadcastType			GetBroadcastType() const;

		void						SetPriority(ACEMIPriority type);
		ACEMIPriority				GetPriority() const;

		void						SetAcknowledgeRequestFlag(bool flag);
		bool						GetAcknowledgeRequestFlag() const;

		void						SetConfirmFlag(bool flag);
		bool						GetConfirmFlag() const;

		void						SetHopCount(uint8 count);
		uint8						GetHopCount() const;

		void						SetExtendedFrameFormat(uint8 format);
		uint8						GetExtendedFrameFormat() const;

		void						SetAddressType(AAddressType type);
		AAddressType				GetAddressType() const;

		void						SetSource(const AIndividualAddress& address);
		const AIndividualAddress&	GetSource() const;

		void						SetDestinationIndividual(const AIndividualAddress& address);
		const AIndividualAddress&	GetDestinationInduvidual() const;

		void						SetDestinationGroup(const AGroupAddress& address);
		const AGroupAddress&		GetDestinationGroup() const;

		void						SetTPCI(uint8 value);
		uint8						GetTPCI() const;

		void						SetAPCI(ACEMIAPCI value);
		ACEMIAPCI					GetAPCI() const;

		void						SetFirstPayload(uint8 payload);
		uint8						GetFirstPayload() const;

		void						SetPayload(const void* data, uint8 size);
		const void*					GetPayload() const;
		uint8						GetPayloadSize() const;

		void						SetUInt8(uint8 value);
		uint8						GetUInt8() const;

		void						SetUInt16(uint16 value);
		uint16						GetUInt16() const;

		void						SetUInt32(uint32 value);
		uint32						GetUInt32() const;

		void						SetUInt64(uint64 value);
		uint64						GetUInt64() const;

		void						SetInt8(int8 value);
		int8						GetInt8() const;

		void						SetInt16(int16 value);
		int16						GetInt16() const;

		void						SetInt32(int32 value);
		int32						GetInt32() const;

		void						SetInt64(int64 value);
		int64						GetInt64() const;

		void						SetHalf(float value);
		float						GetHalf() const;

		void						SetFloat(float value);
		float						GetFloat() const;

		void						SetDouble(double value);
		double						GetDouble() const;

		void						SetBoolean(bool value);
		bool						GetBoolean() const;

		uint8						GetSize() const;

		void						Generate(void* buffer, uint8& size) const;
		size_t						Process(const void* buffer, size_t size);

		std::string					Print() const;

									ACEMIMessageData();
};