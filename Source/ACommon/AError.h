#pragma once

#define RETURN_VOID

#define Log(Message, ...) do { AConsole::Output(AOutputType::Log, __FUNCTION__, Message, __VA_ARGS__); } while(false)

#define RaiseCriticalError(Message, ...) do { AConsole::Output(AOutputType::CriticalError, "",  Message, __VA_ARGS__); } while(false)
#define RaiseError(Message, ...) do { AConsole::Output(AOutputType::Error, "", Message, __VA_ARGS__); } while(false)
#define RaiseWarning(Message, ...) do { AConsole::Output(AOutputType::Warning, "", Message, __VA_ARGS__); } while(false)

#define CheckCriticalError(Condition, Message, ...) do { if (Condition) { RaiseError(Message, __VA_ARGS__); AConsole::Abort(); }} while(false)
#define CheckError(Condition, Return, Message, ...) do { if (Condition) { RaiseError(Message, __VA_ARGS__); return Return; }} while(false)
#define CheckWarning(Condition, Message, ...) do { if (Condition) { RaiseWarning(Message, __VA_ARGS__); }} while(false)

enum class AOutputType
{
	CriticalError,
	Error,
	Warning,
	Log
};

class AConsole
{
	public:
		static void Output(AOutputType Type, const char* Module, const char* Message, ...);
		static void Abort();
};