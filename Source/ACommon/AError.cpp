#include "AError.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void AConsole::Output(AOutputType Type, const char* Module, const char* Message, ...)
{
	switch(Type)
	{
		case AOutputType::CriticalError:
			printf("[%s] CRITICAL ERROR: ", Module);
			break;

		case AOutputType::Error:
			printf("[%s] Error: ", Module);
			break;

		case AOutputType::Warning:
			printf("[%s] Warning: ", Module);
			break;

		default:
		case AOutputType::Log:
			printf("[%s] Log: ", Module);
			break;
	}

	va_list args;
	va_start(args, Message);
	vprintf(Message, args);
	va_end(args);

	printf("\n");
}

void AConsole::Abort()
{
	abort();
}