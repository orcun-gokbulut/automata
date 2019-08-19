
#include "AAddress.h"
#include "ACommon/ATypes.h"

#include <vector>

enum class ATelegramFrameFormat : uint8
{
	Standard = 2,
	Extended = 1,
	Poll = 3,
};

enum class ATelegramPriority : uint8
{
	System = 0,
	High = 1,
	Alarm = 2,
	Normal = 3
};

enum class ATelegramAddressType : uint8
{
	IndividualAddress = 0,
	GroupAddress = 1
};

enum class ATelegramCommand : uint8
{
	ValueRead = 0,
	ValueResponse = 1,
	ValueWrite = 2,
	MemoryWrite = 10
};

class ATelegram
{
	private:
		bool						repeatFlag;
		ATelegramPriority			priority;
		ATelegramAddressType		addressType;
		AAddress					source;
		AAddress					destination;
		ATelegramCommand			command;
		uint8						firstPayload;
		std::vector<uint8>			payload;

	public:
		void						SetPriority(ATelegramPriority priority);
		ATelegramPriority			GetPriority() const;

		void						SetAddressType(ATelegramAddressType type);
		ATelegramAddressType		GetAddressType() const;

		void						SetSource(const AAddress& address);
		const AAddress&				GetSource() const;

		void						SetDestination(const AAddress& address);
		const AAddress&				GetDestination() const;

		void						SetRepeatFlag(bool flag);
		bool						GetRepeatFlag() const;

		void						SetCommand(ATelegramCommand command);
		ATelegramCommand			GetCommand() const;

		void						SetFirstPayload(uint8 value);
		uint8						GetFirstPayload() const;

		void						SetPayload(const void* data, size_t size);
		const void*					GetPayload() const;
		size_t						GetPayloadSize() const;

		size_t						GetSize();

		void						Generate(void* buffer, size_t& size) const;
		bool						Process(const void* buffer, size_t size);

									ATelegram();
};