// timer.h
// Author: abramann
// Note this file is influenced by GameTimer.h from F.Luna's Introduction To Game Programming in D3D11

#pragma once

#include "types.h"

class Timer
{
public:
	Timer();
	~Timer();
	int64 getCurrentTime();
	int16 getFPS() { return m_fps; }
	void update();
	float getTimeDelta() { return m_timeDeltaMillsec; }
	float getTimeFactor() { return m_expectedFrameTimeMillsec*1.0f / m_timeDeltaMillsec*1.0f; }

private:

	void fpsUpdate();
	void sleep(int32 millsec);
	void spinLock(int32 millsec);

	int64 getCurrentCounts() const;
	float m_timeDeltaMillsec;
	int64 m_prevTime;
	int32 m_expectedFrameTimeMillsec;
	int16 m_fps;
	int32 m_fpsUpdateDelay;
#ifdef WIN32
	int64 m_countsPerMillisecond;
#endif
};
