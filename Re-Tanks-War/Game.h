// Author: Wefaq
// Breaf: The shared client / server interface.
 #pragma once

#include "Types.h"
#include "INI.h"
#include "Window.h"

class IRenderer;
class ITime;

class IGame
{
public:
	void start()
	{
		initialize();

		run();

		shutdown();
	}

private:
	virtual void initialize() = 0;
	virtual void run() = 0;
	virtual void shutdown() = 0;
};

extern IGame* g_pGame;