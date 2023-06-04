// timer.cpp
// Author: abramann
// Note this file is influenced by GameTimer.h from F.Luna's Introduction To Game Programming in D3D11

#include "timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSecond);
	m_secondsPerCount = 1.0f / m_countsPerSecond;
	m_minFrameTime = 1000 / gameNS::FRAME_RATE;
	m_maxFrameTime = m_minFrameTime * 10;
	m_prevCounts = getCurrentCounts();
}

Timer::~Timer()
{
}

float Timer::getCurrentTime()
{
	int64 counts = getCurrentCounts();
	float currentTime = counts* m_secondsPerCount * 1000;
	return currentTime;
}

void Timer::update()
{
	int64 currentCounts = getCurrentCounts();
	m_timeDeltaMillsec = (currentCounts - m_prevCounts)*m_secondsPerCount * 1000; // Convert micro to mill
	m_prevCounts = currentCounts;

	static float fpsUpdateDelay = 0;
	fpsUpdateDelay += m_timeDeltaMillsec;

	if (fpsUpdateDelay >= gameNS::UPDATE_DELAY_FPS)
	{
		m_fps = 1000.0f / m_timeDeltaMillsec;
		fpsUpdateDelay = 0;
	}

	if (m_timeDeltaMillsec < m_minFrameTime)
	{
		int32 sleepTime = m_minFrameTime - round(m_timeDeltaMillsec);
		Sleep(sleepTime);
		m_timeDeltaMillsec = m_minFrameTime;
	}
	//	else if (m_timeDeltaMillsec > m_maxFrameTime)
	//		m_timeDeltaMillsec = m_maxFrameTime;
}

int64 Timer::getCurrentCounts() const
{
	int64 counts = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return counts;
}