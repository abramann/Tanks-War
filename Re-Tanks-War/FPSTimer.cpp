#include "ITime.h"
#include "FPSTimer.h"
#include "Values.h"

static CFPSTimer fpsTimer;
CFPSTimer* g_pFPSTimer = &fpsTimer;

void CFPSTimer::update()
{
	updateFrameTime();

	updateFPS();
}

void CFPSTimer::reset()
{
	m_fps = 0;

	s_frameTime = 0;
}

void CFPSTimer::updateFrameTime()
{
	int minFrameTime = 1000 / m_refRate;

	if (s_frameTime < minFrameTime)
	{
		int64_t sleepTime = static_cast<int64_t>((round(minFrameTime * 10) / 10) - s_frameTime);

		s_frameTime += sleepTime;

		g_pTime->sleep(sleepTime);
	}
}

void CFPSTimer::updateFPS()
{
	m_spentFPSTime += s_frameTime;

	if (m_spentFPSTime >= 1000)
	{
		m_fps = 0;

		m_spentFPSTime = 0;
	}
	else
		m_fps++;
}
