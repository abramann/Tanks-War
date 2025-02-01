#pragma once

#include "Game.h"

class CClient : public IGame
{
	void shutdown() override;
private:
	void initialize() override;
	void run() override;
};