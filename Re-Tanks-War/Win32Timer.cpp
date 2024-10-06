#include "Win32Timer.h"
#include "GameData.h"
#include <chrono>
#include <thread>
#include <Windows.h>


using namespace nsGameConfig;


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
	if (timeDelta < FRAME_TIME)
	{
		 int64_t sleepTime = static_cast<int64_t>((round(FRAME_TIME * 10) / 10) - timeDelta);
		 timeDelta += sleepTime;
		 this->sleep(sleepTime);
	}

	m_timeFactor = static_cast<float>(FRAME_RATE / timeDelta);
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

	timeBeginPeriod(1);
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		now = getTime();
		delta = now - start;
	} while (delta < millsec);
	timeEndPeriod(1);
}
