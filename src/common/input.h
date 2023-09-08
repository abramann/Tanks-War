// input.h
// Author: abramann

#pragma once

#include "types.h"

class Input
{
public:

	Input();
	~Input();
	bool initialize(HWND hwnd);
	void handle(UINT msg, WPARAM wParam, LPARAM lParam);
	bool isKeyDown(Key key);
	bool isKeyPressed(Key key);
	bool isKeyReleased(Key key);
	bool isInputHandled() const { return m_handleInput; }

private:

	HWND m_hwnd;
	bool m_handleInput;
};