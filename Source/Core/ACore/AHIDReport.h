#pragma once

#include "ACommon/ATypes.h"

#include <string>

enum class AHIDProtocolId : uint8
{
	KNXTunnel = 0x01,
	BusAccessServerFeatureService = 0x0F,
};

class AHIDReport
{
	friend class ACore;
	private:
		mutable uint64				m_index;
		uint8						m_reportID;
		bool						m_startPacket;
		bool						m_endPacket;
		bool						m_partialPacket;
		uint8						m_sequenceNumber;
		uint8						m_protocolVersion;
		AHIDProtocolId				m_protocolId;
		uint8						m_EMIId;
		uint16						m_manufacturerCode;
		uint8						m_dataSize;
		uint8						m_data[256];

		void						SetIndex(uint64 index) const;

	public:
		uint64						GetIndex() const;

		void						SetReportID(uint8 id);
		uint8						GetReportID() const;

		void						SetStartPacket(bool flag);
		bool						GetStartPacket() const;
		void						SetEndPacket(bool flag);
		bool						GetEndPacket() const;
		void						SetPartialPacket(bool flag);
		bool						GetPartialPacket() const;
		void						SetSequenceNumber(uint8 number);
		uint8						GetSequenceNumber() const;

		void						SetProtocolVersion(uint8 version);
		uint8						GetProtocolVersion() const;

		void						SetProtocolId(AHIDProtocolId protocolId);
		AHIDProtocolId				GetProtocolId() const;

		void						SetEMIId(uint8 EMIId);
		uint8						GetEMIId() const;

		void						SetManufacturerCode(uint16 manufacturerCode);
		uint16						GetManufacturerCode() const;

		void						SetDataSize(uint8 size);
		uint8						GetDataSize() const;

		void						SetData(const void* data, uint8 size);
		void						AddData(const void* data, uint8 size);
		const void*					GetData() const;

		void						Generate(void* buffer, uint8& size) const;
		bool						Process(const void* buffer, uint8 size);

		void						Reset();

		std::string					ToString() const;

									AHIDReport();
};
