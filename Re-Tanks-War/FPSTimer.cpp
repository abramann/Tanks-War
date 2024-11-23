#include "ITime.h"
#include "FPSTimer.h"
#include "Values.h"

static CFPSTimer fpsTimer;
CFPSTimer* g_pFPSTimer = &fpsTimer;

void CFPSTimer::update()
{
	if (s_frameTime < values::FRAME_TIME)
	{
		int64_t sleepTime = static_cast<int64_t>((round(values::FRAME_TIME * 10) / 10) - s_frameTime);
		s_frameTime += sleepTime;
		g_pTime->sleep(sleepTime);
	}
}

void CFPSTimer::reset()
{
}
