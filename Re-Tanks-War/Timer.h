#pragma once

#include "SystemComponent.h"

class ITimer : public ISystemComponent
{
public:
	static uint32_t s_frameTime;
	
	ITimer() : m_delta(0) {}
	std::string getName() const override { return "Timer"; };
	uint32_t timeDelta() const { return m_delta; }

private:
	uint32_t m_delta;
};
