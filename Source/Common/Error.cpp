#include "Error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void Console::Output(OutputType Type, const char* Module, const char* Message, ...)
{
	switch(Type)
	{
		case OutputType::CriticalError:
			printf("[%s] CRITICAL ERROR: ", Module);
			break;

		case OutputType::Error:
			printf("[%s] Error: ", Module);
			break;

		case OutputType::Warning:
			printf("[%s] Warning: ", Module);
			break;

		default:
		case OutputType::Log:
			printf("[%s] Log: ", Module);
			break;
	}

	va_list args;
	va_start(args, Message);
	vprintf(Message, args);
	va_end(args);

	printf("\n");
}

void Console::Abort()
{
	abort();
}