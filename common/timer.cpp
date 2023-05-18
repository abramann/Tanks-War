#include "timer.h"

Timer::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_countsPerSecond);
	m_secondsPerCount = 1.0f / m_countsPerSecond;
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

	if (fpsUpdateDelay >= UPDATE_DELAY_FPS)
	{
		m_fps = 1000.0f / m_timeDeltaMillsec;
		fpsUpdateDelay = 0;
	}

	if (m_timeDeltaMillsec < MIN_FRAME_TIME)
	{
		int32 sleepTime = MIN_FRAME_TIME - round(m_timeDeltaMillsec);
		Sleep(sleepTime);
		m_timeDeltaMillsec = MIN_FRAME_TIME;
	}
	else if (m_timeDeltaMillsec > MAX_FRAME_TIME)
		m_timeDeltaMillsec = MAX_FRAME_TIME;
}

int64 Timer::getCurrentCounts() const
{
	int64 counts = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return counts;
}
