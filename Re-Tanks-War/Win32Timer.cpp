#include "Win32Timer.h"
#include "Values.h"
#include <Windows.h>

static CWin32Timer timer;
ITime* g_pTime = &timer;

void CWin32Timer::startup()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	m_countsPerMillisecond = frequency.QuadPart / 1000;
}

int64_t CWin32Timer::getTime() const
{
	int64_t counts;
	QueryPerformanceCounter((LARGE_INTEGER*)&counts);
	return static_cast<int64_t>(counts / m_countsPerMillisecond);
}

