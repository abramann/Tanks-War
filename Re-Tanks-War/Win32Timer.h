#pragma once

#include "ITime.h"

class CWin32Timer : public ITime
{
public:
	void startup() override;
	int64_t getTime() const override;

private:
	int64_t m_countsPerMillisecond;
};
