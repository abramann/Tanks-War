#ifndef _INPUT_H
#define _INPUT_H
#include "constants.h"

class Input
{

public:
	
	Input();
	~Input();
	bool initialize(HWND hWnd);
	void handle(UINT msg, WPARAM wParam, LPARAM lParam);
	bool isKeyIn(Key k);
	bool anyKeyPressed();

	bool m_handleInput;

private:

	HWND m_hwnd;

};


#endif