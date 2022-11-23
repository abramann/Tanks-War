#ifndef _GAME_H
#define _GAME_H
#include "always.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "image.h"
#include "audio.h"
#include "textDX.h"
#include <timeapi.h>

class Game
{
public:

	Game();
	~Game();

	LRESULT messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen);
	virtual void run();
	virtual void renderGame();
	virtual void updateGame();

	virtual void handleLostGraphicsDevice();
	virtual void onLostDevice();
	virtual void onResetDevice();

	virtual void collision() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	Graphics* getGraphics()	{ return graphics; }
	Input* getInput()	{ return input; }
	Audio* getAudio()	{ return audio; }

protected:

	Graphics* graphics;
	Input* input;
	Audio* audio;
	HWND hWnd;
	bool fullscreen;
	DWORD timeDelta;

};


#endif
