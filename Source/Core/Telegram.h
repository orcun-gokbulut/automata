
#include "Address.h"
#include "Common/Types.h"

#include <vector>

enum class OHTelegramFrameFormat : uint8
{
	Standard = 2,
	Extended = 1,
	Poll = 3,
};

enum class OHTelegramPriority : uint8
{
	System = 0,
	High = 1,
	Alarm = 2,
	Normal = 3
};

enum class OHTelegramAddressType : uint8
{
	IndividualAddress = 0,
	GroupAddress = 1
};

enum class OHTelegramCommand : uint8
{
	ValueRead = 0,
	ValueResponse = 1,
	ValueWrite = 2,
	MemoryWrite = 10
};

class OHTelegram
{
	private:
		bool						repeatFlag;
		OHTelegramPriority			priority;
		OHTelegramAddressType		addressType;
		OHAddress					source;
		OHAddress					destination;
		OHTelegramCommand			command;
		uint8						firstPayload;
		std::vector<uint8>			payload;

	public:
		void						SetPriority(OHTelegramPriority priority);
		OHTelegramPriority			GetPriority() const;

		void						SetAddressType(OHTelegramAddressType type);
		OHTelegramAddressType		GetAddressType() const;

		void						SetSource(const OHAddress& address);
		const OHAddress&			GetSource() const;

		void						SetDestination(const OHAddress& address);
		const OHAddress&			GetDestination() const;

		void						SetRepeatFlag(bool flag);
		bool						GetRepeatFlag() const;

		void						SetCommand(OHTelegramCommand command);
		OHTelegramCommand			GetCommand() const;

		void						SetFirstPayload(uint8 value);
		uint8						GetFirstPayload() const;

		void						SetPayload(const void* data, size_t size);
		const void*					GetPayload() const;
		size_t						GetPayloadSize() const;

		void						Generate(void* buffer, size_t& size) const;
		bool						Process(const void* buffer, size_t size);

									OHTelegram();
};