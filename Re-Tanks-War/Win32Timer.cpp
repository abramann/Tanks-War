#include "Win32Timer.h"
#include "Values.h"
#include <chrono>
#include <thread>
#include <Windows.h>

static CWin32Timer timer;
ITimer* g_pTimer = &timer;

void CWin32Timer::startup()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	m_countsPerMillisecond = frequency.QuadPart / 1000;
	m_prevTime = getTime();
}

void CWin32Timer::update()
{
	int64_t currentTime = getTime();
	int64_t timeDelta = currentTime - m_prevTime;
	m_prevTime = currentTime;
	if (timeDelta < values::FRAME_TIME)
	{
		 int64_t sleepTime = static_cast<int64_t>((round(values::FRAME_TIME * 10) / 10) - timeDelta);
		 timeDelta += sleepTime;
		 this->sleep(sleepTime);
	}

	m_timeFactor = static_cast<float>(values::FRAME_RATE / timeDelta);
}

float CWin32Timer::getTimeFactor() const
{
	return m_timeFactor;
}

int64_t CWin32Timer::getTime() const
{
	int64_t counts;
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return static_cast<int64_t>(counts / m_countsPerMillisecond);
}

void CWin32Timer::sleep(int64_t millsec) const
{
	int64_t start = getTime();
	int64_t now = 0;
	int64_t delta = 0;

	do
	{
		timeBeginPeriod(1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		timeEndPeriod(1);
		now = getTime();
		delta = now - start;
	} while (delta < millsec);
}
