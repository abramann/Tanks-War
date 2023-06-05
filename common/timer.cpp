// timer.cpp
// Author: abramann
// Note this file is influenced by GameTimer.h from F.Luna's Introduction To Game Programming in D3D11

#include "timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSecond);
	m_secondsPerCount = 1.0f / m_countsPerSecond;
	m_expectedFrameTimeMillsec = 1000 / gameNS::FRAME_RATE;
	m_prevCounts = getCurrentCounts();
	m_fpsUpdateDelay = gameNS::UPDATE_DELAY_FPS;
}

Timer::~Timer()
{
}

float Timer::getCurrentTime()
{
	int64 counts = getCurrentCounts();
	float currentTime = counts * m_secondsPerCount * 1000;
	return currentTime;
}

void Timer::update()
{
	int64 currentCounts = getCurrentCounts();
	m_timeDeltaMillsec = (currentCounts - m_prevCounts)*m_secondsPerCount * 1000; // Convert micro to mill
	m_prevCounts = currentCounts;
	if (m_timeDeltaMillsec < m_expectedFrameTimeMillsec)
	{
		int32 sleepTime = m_expectedFrameTimeMillsec - round(m_timeDeltaMillsec);
		this->sleep(sleepTime*2);
		m_timeDeltaMillsec = m_expectedFrameTimeMillsec;
	}

	m_fpsUpdateDelay += m_timeDeltaMillsec;
	if (m_fpsUpdateDelay >= gameNS::UPDATE_DELAY_FPS)
		fpsUpdate();
}

void Timer::fpsUpdate()
{
	m_fps = 1000.0f / m_timeDeltaMillsec;
	m_fpsUpdateDelay = 0;
}

void Timer::sleep(int32 millsec)
{
	Sleep(millsec);
}

int64 Timer::getCurrentCounts() const
{
	int64 counts = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return counts;
}