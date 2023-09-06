// timer.cpp
// Author: abramann
// Note this file is influenced by GameTimer.h from F.Luna's Introduction To Game Programming in D3D11

#include "timer.h"
#include <timeapi.h>
#include <thread>
#ifndef WIN32
#include <chrono>

using namespace std;
using namespace chrono;
#endif

Timer::Timer() : m_fpsUpdateDelay(0), m_fps(0)
{
#ifdef WIN32
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	m_countsPerMillisecond = frequency.QuadPart / 1000;
#else

#endif
	m_expectedFrameTimeMillsec = (1000 / gameNS::FRAME_RATE) * 2;
	m_prevTime = getCurrentTime();
}

Timer::~Timer()
{
}

int64 Timer::getCurrentTime()
{
	int64 currentTime;
#ifdef WIN32
	int64 counts = getCurrentCounts();
	currentTime = counts / m_countsPerMillisecond;
#else
	currentTime = chrono::duration_cast<milliseconds>(high_resolution_clock().now().time_since_epoch()).count();
#endif
	return currentTime;
}

void Timer::update()
{
	int64 currentTime = getCurrentTime();
	m_timeDeltaMillsec = currentTime - m_prevTime;
	m_prevTime = currentTime;
	if (!g_pGameSettings->vsync && m_timeDeltaMillsec < m_expectedFrameTimeMillsec)
	{
		auto sleepTime = m_expectedFrameTimeMillsec - m_timeDeltaMillsec;
		auto before = getCurrentTime();
		this->sleep(sleepTime);
		auto sleepSpend = getCurrentTime() - before;
		m_timeDeltaMillsec += sleepSpend;
	}

	m_fpsUpdateDelay += m_timeDeltaMillsec;
	if (m_fpsUpdateDelay >= gameNS::UPDATE_DELAY_FPS)
		fpsUpdate();
}

void Timer::fpsUpdate()
{
	m_fps = static_cast<int32>(1000 / m_timeDeltaMillsec);
	m_fpsUpdateDelay = 0;
}

void Timer::spinLock(int32 millsec)
{
	int64 start = getCurrentTime();
	int64 now = 0, delta = 0;
	do
	{
		now = getCurrentTime();
		delta = now - start;
	} while (delta < millsec);
}

void Timer::sleep(int64 millsec)
{
	int64 start = getCurrentTime();
	int64 now = 0, delta = 0;
#ifdef WIN32
	timeBeginPeriod(1);
#endif
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		now = getCurrentTime();
		delta = now - start;
	} while (delta < millsec);
#ifdef WIN32
	timeEndPeriod(1);
#endif
}

#ifdef WIN32
int64 Timer::getCurrentCounts() const
{
	LARGE_INTEGER counts = {};
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return counts.QuadPart;
}
#endif