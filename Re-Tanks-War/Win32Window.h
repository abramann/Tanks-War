#pragma once

#include "Window.h"

class CWin32Window : public IWindow
{
public:
	bool pollMessages() override;
private:
	HWindow createHandle(int w, int h, bool fullscreen, const std::string& title,
		bool borderless) override;
	void deleteHandle(HWindow handle) override;
};

