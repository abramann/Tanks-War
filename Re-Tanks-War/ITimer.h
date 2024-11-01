#pragma once

#include <iostream>

class ITimer
{
public:
	virtual void startup() = 0;
	virtual void update() = 0;
	virtual float getTimeFactor() const = 0;
	virtual int64_t getTime() const = 0;
	virtual void sleep(int64_t millsec) const = 0;
};

extern std::shared_ptr<ITimer> g_pTimer;