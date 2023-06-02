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

private:

	int64 getCurrentCounts() const;
	float m_timeDeltaMillsec;
	int64 m_countsPerSecond;
	float m_secondsPerCount;
	int64 m_prevCounts;
	float m_maxFrameTime, m_minFrameTime;
	int16 m_fps;
};

