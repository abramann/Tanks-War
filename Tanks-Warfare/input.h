#ifndef _INPUT_H
#define _INPUT_H
#include "always.h"
#include <dinput.h>

#define A_KEY DIK_A
#define D_KEY DIK_D
#define S_KEY DIK_S
#define W_KEY DIK_W
#define ENTER_KEY DIK_RETURN
#define ESCAPE_KEY DIK_ESCAPE

class Input
{
public:

	Input();
	~Input();
	HRESULT initialize(HINSTANCE hInstance, HWND hWnd);
	void scan();
	void keyDown(BYTE k);
	void reset();
	bool isKeyIn(BYTE k);
	bool anyKeyPressed();

private:

	LPDIRECTINPUT8 directInput;
	LPDIRECTINPUTDEVICE8 keyboard, mouse;
	BYTE keyboardState[256];

};


#endif