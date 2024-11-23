#pragma once

#include "Timer.h"

class CFPSTimer : public ITimer
{
public:
	void update() override;
	void reset() override;
};

extern CFPSTimer* g_pFPSTimer;
