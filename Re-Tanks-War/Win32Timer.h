#pragma once

#include "ITimer.h"

class CWin32Timer : public ITimer
{
public:
	virtual void startup() override;
	virtual void update() override;
	virtual float getTimeFactor() const override;
	virtual int64_t getTime() const override;
	virtual void sleep(int64_t millsec) const override;

private:
	int64_t m_prevTime;
	int64_t m_countsPerMillisecond;
	float m_timeFactor;
};
