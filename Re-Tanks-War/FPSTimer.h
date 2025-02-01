#pragma once

#include "Timer.h"

class CFPSTimer : public ITimer
{
public:
	void update() override;
	void reset() override;

	void updateFrameTime();
	void updateFPS();

private:
	int m_fps;
	int m_refRate;
	int m_spentFPSTime;
};

extern CFPSTimer* g_pFPSTimer;
