// timer.h
// Author: abramann
// Note this file is influenced by GameTimer.h from F.Luna's Introduction To Game Programming in D3D11

#include "constants.h"

class Timer
{
public:
	Timer();
	~Timer();
	float getCurrentTime();
	int16 getFPS() { return m_fps; }
	void update();
	float getTimeDelta() { return m_timeDeltaMillsec; }
	float getTimeFactor() { return m_expectedFrameTimeMillsec*1.0f / m_timeDeltaMillsec*1.0f; }

private:

	void fpsUpdate();
	void sleep(int32 millsec);

	int64 getCurrentCounts() const;
	float m_timeDeltaMillsec;
	int64 m_countsPerSecond;
	float m_secondsPerCount;
	int64 m_prevCounts;
	int32 m_expectedFrameTimeMillsec;
	int16 m_fps;
	int32 m_fpsUpdateDelay;
};
