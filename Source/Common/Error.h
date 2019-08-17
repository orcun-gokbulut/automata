#pragma once

#define RETURN_VOID

#define Log(Message, ...) do { Console::Output(OutputType::Log, "", Message, __VA_ARGS__); } while(false)

#define RaiseCriticalError(Message, ...) do { Console::Output(OutputType::CriticalError, "",  Message, __VA_ARGS__); } while(false)
#define RaiseError(Message, ...) do { Console::Output(OutputType::Error, "", Message, __VA_ARGS__); } while(false)
#define RaiseWarning(Message, ...) do { Console::Output(OutputType::Warning, "", Message, __VA_ARGS__); } while(false)

#define CheckCriticalError(Condition, Message, ...) do { if (Condition) { RaiseError(Message, __VA_ARGS__); Console::Abort(); }} while(false)
#define CheckError(Condition, Return, Message, ...) do { if (Condition) { RaiseError(Message, __VA_ARGS__); return Return; }} while(false)
#define CheckWarning(Condition, Message, ...) do { if (Condition) { RaiseWarning(Message, __VA_ARGS__); }} while(false)

enum class OutputType
{
	CriticalError,
	Error,
	Warning,
	Log
};

class Console
{
	public:
		static void Output(OutputType Type, const char* Module, const char* Message, ...);
		static void Abort();
};