#pragma once

#include "System.h"
#include <memory>

class CViewSystem : public ISystem
{
public:
	CViewSystem();

	void startup() override;
	void update() override;
	void reset() override;
};

extern std::shared_ptr<CViewSystem> g_pViewSystem;

