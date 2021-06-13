#include "AError.h"

#include <iostream>

#include <stdlib.h>
#include <stdarg.h>

using namespace std;

void AConsole::Output(AOutputType Type, const char* Module, const char* Message, ...)
{
	cout << "[" << Module << "] ";
	switch (Type)
	{
		case AOutputType::CriticalError:
			cout << "CRITICAL ERROR: ";
			break;

		case AOutputType::Error:
			cout << "Error: ";
			break;

		case AOutputType::Warning:
			cout << "Warning: ";
			break;

		default:
		case AOutputType::Log:
			cout << "Log: ";
			break;
	};

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