#pragma once

#include "TankModel.h"


class CTankModel;

namespace nsModuleBuilder
{
	enum TankModels
	{
		TANK_SCORPION,
		TANK_CRAUSDER,
		TANK_GATTING,
		TANK_OVERLORD,
	}; 
	
	CTankModel* buildTankModel(int tankModelType);
};

