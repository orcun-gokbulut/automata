#pragma once

#include "Address.h"

#include <string>

using namespace std;

class Device
{
	private:
		string				serialNumber;
		string				name;

	public:
		void				SetName(const string& name);
		const string		GetName() const;

		void				SetEventHandler();
};