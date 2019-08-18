#pragma once

#include "DataPoint.h"

#include <string>
#include <vector>

using namespace std;

class OHDevice
{
	private:
		uint32								serialNumber;
		string								name;

		std::vector<OHDataPoint*>			dataPoints;

	public:
		void								SetSerialNumber(uint32 number);
		int									GetSerialNumber() const;

		void								SetName(const string& name);
		const string						GetName() const;

		const std::vector<OHDataPoint*>&	GetDataPoints() const;
		void								AddDataPoint(OHDataPoint* dataPoint);
		void								RemoveDataPoint(OHDataPoint* dataPoint);

		virtual void						Process();

											OHDevice();
		virtual								~OHDevice();
};