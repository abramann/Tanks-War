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
	bool isKeyDown(Key k);
	bool isKeyPressed(Key k);
	bool anyKeyPressed();

	bool m_handleInput;

private:

	HWND m_hwnd;
};