#pragma once

#include "Types.h"

class IWindow
{
public:
	void create();
	void reset();
	void destroy();
	HWindow getHandle() const;

	virtual bool pollMessages() = 0;

private:
	virtual HWindow createHandle(int w, int h, bool fullscreen, const std::string& title,
						bool borderless) = 0;
	virtual void deleteHandle(HWindow handle) = 0;

	HWindow m_handle;
};

extern IWindow* g_pWindow;