#ifndef _GAME_H
#define _GAME_H
#include "constants.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "image.h"
#include "audio.h"

class Game
{
public:

	Game();
	~Game();

	LRESULT messageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void initialize(HINSTANCE hInstance, HWND hWnd);
	virtual void run();
	virtual void renderGame();
	virtual void updateGame();

	virtual void handleLostGraphicsDevice();
	virtual void onLostDevice();
	virtual void onResetDevice();

	virtual void collision() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	Graphics* getGraphics()	{ return m_pGraphics; }
	Input* getInput()	{ return m_pInput; }
	Audio* getAudio()	{ return m_pAudio; }

protected:

	Graphics* m_pGraphics;
	Input* m_pInput;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;

	DWORD m_timeDelta;

};


#endif
