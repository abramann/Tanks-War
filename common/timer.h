#pragma once
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
	int16 m_fps;
};

