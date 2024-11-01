#pragma once

#include "System.h"
#include <memory>

class CLevelSystem : ISystem
{
public:
	void startup() override;
	void update() override;
	void reset() override;
	void perform() override;

};

extern std::shared_ptr<CLevelSystem> g_pLevelSystem;