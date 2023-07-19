// timer.cpp
// Author: abramann
// Note this file is influenced by GameTimer.h from F.Luna's Introduction To Game Programming in D3D11

#include "timer.h"
#include <timeapi.h>
#include <thread>
#ifndef PLATFORM_WINDOWS
#include <chrono>

using namespace std;
using namespace chrono;
#endif

Timer::Timer()
{
#ifdef PLATFORM_WINDOWS
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	m_countsPerMillisecond = frequency.QuadPart / 1000;
#else
	
#endif
	m_expectedFrameTimeMillsec = (1000 / gameNS::FRAME_RATE) * 2;
	m_prevTime = getCurrentTime();
	m_fpsUpdateDelay = gameNS::UPDATE_DELAY_FPS;
}

Timer::~Timer()
{
}

int64 Timer::getCurrentTime()
{
	int64 currentTime;
#ifdef PLATFORM_WINDOWS
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
	if (m_timeDeltaMillsec < m_expectedFrameTimeMillsec)
	{
		int32 sleepTime = m_expectedFrameTimeMillsec - m_timeDeltaMillsec;
		int64 before = getCurrentTime();
		this->sleep(sleepTime);
		auto sleepSpend = getCurrentTime() - before;
		m_timeDeltaMillsec += sleepSpend;
	}

	m_fpsUpdateDelay += m_timeDeltaMillsec;
	//if (m_fpsUpdateDelay >= gameNS::UPDATE_DELAY_FPS)
		fpsUpdate();
}

void Timer::fpsUpdate()
{
	m_fps = 1000.0f / m_timeDeltaMillsec;
	static int fpsUp = 0;
	if (m_fps > 65)
		fpsUp++;
	m_fpsUpdateDelay = 0;
}

void Timer::spinLock(int32 millsec)
{
	/*double seconds = millsec*1.0f / 1000.0f;
	static HANDLE timer = CreateWaitableTimer(NULL, FALSE, NULL);
	static double estimate = 5e-3;
	static double mean = 5e-3;
	static double m2 = 0;
	static int64_t count = 1;

	while (seconds - estimate > 1e-7) {
		double toWait = seconds - estimate;
		LARGE_INTEGER due;
		due.QuadPart = -int64_t(toWait * 1e7);
		auto start = high_resolution_clock::now();
		SetWaitableTimerEx(timer, &due, 0, NULL, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		auto end = high_resolution_clock::now();

		double observed = (end - start).count() / 1e9;
		seconds -= observed;

		++count;
		double error = observed - toWait;
		double delta = error - mean;
		mean += delta / count;
		m2 += delta * (error - mean);
		double stddev = sqrt(m2 / (count - 1));
		estimate = mean + stddev;
	}

	// spin lock
	auto start = high_resolution_clock::now();
	auto spinNs = int64_t(seconds * 1e9);
	auto delay = nanoseconds(spinNs);
	while (high_resolution_clock::now() - start < delay);
	*/

	int64 start = getCurrentTime();
	int64 now = 0, delta = 0;
	do
	{
		now = getCurrentTime();
		delta = now - start;
	} while (delta < millsec);
}

void Timer::sleep(int32 millsec)
{
	int64 start = getCurrentTime();
	int64 now = 0, delta = 0;
#ifdef PLATFORM_WINDOWS
	timeBeginPeriod(1);
#endif
	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		now = getCurrentTime();
		delta = now - start;
	}
	while (delta < millsec);
#ifdef PLATFORM_WINDOWS
	timeEndPeriod(1);
#endif
}

#ifdef PLATFORM_WINDOWS
int64 Timer::getCurrentCounts() const
{
	LARGE_INTEGER counts = {};
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return counts.QuadPart;
}
#endif