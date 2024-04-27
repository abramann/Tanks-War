#pragma once

#include "types.h"
#include <memory>




class IRenderer;
class ITimer;

class IGame
{
public:
	virtual void initialize() = 0;
	virtual void run() = 0;
	virtual void shutdown() = 0;
	virtual bool pollMessages() = 0;
	virtual LRESULT messageHandler(HWindow hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual int getWindowWidth() const = 0;
	virtual int getWindowHeight() const = 0;
	virtual bool isWindowed() const = 0;

private:
	virtual void createWindow() = 0;
	virtual void readInitializeSettings() = 0;
	virtual void update() = 0;
	virtual void renderScene() = 0;
};

extern IGame* g_pGame;