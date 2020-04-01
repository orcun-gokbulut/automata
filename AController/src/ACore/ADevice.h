#pragma once

#include "ADataPoint.h"

#include <string>
#include <vector>

class ACEMIMessage;
class ACore;
class ADevice
{
	friend class ACore;
	friend class ADataPoint;
	private:
		ACore*								core;
		uint32								serialNumber;
		string								name;
		bool								initialized;

		std::vector<ADataPoint*>			dataPoints;

		void								DataPointChanged(const ADataPoint& dataPoint);

	protected:
		const std::vector<ADataPoint*>&		GetDataPoints() const;
		void								RegisterDataPoint(ADataPoint* dataPoint);
		void								UnregisterDataPoint(ADataPoint* dataPoint);

	public:
		ACore*								GetCore() const;

		void								SetSerialNumber(uint32 number);
		int									GetSerialNumber() const;

		void								SetName(const string& name);
		const char*							GetName() const;
		
		bool								IsInitialized();
		virtual bool						Initialize();
		virtual bool						Deinitialize();

		virtual void						TelegramReceived(const ACEMIMessage& message);

		virtual void						PreProcess();
		virtual void						PostProcess();

											ADevice();
		virtual								~ADevice();
};