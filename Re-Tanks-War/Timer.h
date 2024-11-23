#pragma once

#include "SystemComponent.h"

class CTimer : public ISystemComponent
{
public:
	static uint32_t s_frameTime;
	
	std::string getName() const override { return "Timer"; };
	void update() override;
	void reset() override;
	uint32_t timeDelta() const;

private:
	uint32_t m_delta;
};

