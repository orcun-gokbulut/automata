#pragma once

#include "ADataPoint.h"

#include <string>
#include <vector>

class ACEMIMessage;
class ACore;
class ADevice
{
	friend class ACore;
	private:
		ACore*								core;
		uint32								serialNumber;
		string								name;
		bool								initialized;

		std::vector<ADataPoint*>			dataPoints;

	public:
		ACore*								GetCore() const;

		void								SetSerialNumber(uint32 number);
		int									GetSerialNumber() const;

		void								SetName(const string& name);
		const char*							GetName() const;

		const std::vector<ADataPoint*>&		GetDataPoints() const;
		void								AddDataPoint(ADataPoint* dataPoint);
		void								RemoveDataPoint(ADataPoint* dataPoint);
		
		bool								IsInitialized();
		virtual bool						Initialize();
		virtual bool						Deinitialize();

		virtual void						TelegramReceived(const ACEMIMessage* message);
		virtual void						DataPointChanged(const ADataPoint& dataPoint);

		virtual void						PreProcess();
		virtual void						PostProcess();

											ADevice();
		virtual								~ADevice();
};