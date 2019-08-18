#pragma once

#include "DataPoint.h"

#include <string>
#include <vector>

using namespace std;

class ADevice
{
	private:
		uint32								serialNumber;
		string								name;

		std::vector<ADataPoint*>			dataPoints;

	public:
		void								SetSerialNumber(uint32 number);
		int									GetSerialNumber() const;

		void								SetName(const string& name);
		const string						GetName() const;

		const std::vector<ADataPoint*>&		GetDataPoints() const;
		void								AddDataPoint(ADataPoint* dataPoint);
		void								RemoveDataPoint(ADataPoint* dataPoint);

		virtual void						Process();

											ADevice();
		virtual								~ADevice();
};