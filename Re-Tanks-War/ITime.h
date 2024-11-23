#pragma once

#include <iostream>

class ITime
{
public:
	virtual void startup() = 0;
	virtual int64_t getTime() const = 0;

	void sleep(int64_t millsec) const;
};

extern ITime* g_pTime;
