#ifndef _GAME_H
#define _GAME_H
#include "constants.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "image.h"
#include "audio.h"
#include "interface.h"
#include "map2.h"

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

	Graphics* getGraphics()	const { return m_pGraphics; }
	Input* getInput()	const { return m_pInput; }
	Audio* getAudio()	const { return m_pAudio; }
	TextureManger* getTextureManger() const { return m_pTextureManger; }
	Map2* getMap()const { return m_pMap; }

protected:

	Graphics* m_pGraphics;
	Input* m_pInput;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;
	Interface* m_pInterface;
	Map2* m_pMap;

	int64_t m_startTime, m_endTime, m_freq;
	float m_timeDeltaMs;
	float m_fps;
	Image m_logo;
};

#endif
