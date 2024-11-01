#pragma once

#include "System.h"
#include <memory>

class CEventSystem : public ISystem
{
public:
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;
};

extern std::shared_ptr<CEventSystem> m_pEventSystem;
