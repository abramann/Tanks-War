// game.h
// Author: abramann
// Note this file is influnced by game.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0 
// Note parts of this code are licensed under CC BY 3.0

// Programming 2D Games Copyright (c) 2011 by: Charles Kelly 
// game.h

#ifndef _GAME_H
#define _GAME_H
#include "constants.h"
/*#include "graphics.h"
#include "input.h"
#include "timer.h"
#include "texture.h"
#include "image.h"
#include "audio.h"
#include "interface.h"
#include "map.h"*/
#ifdef _CLIENT_BUILD
#include "..\Tanks-War\client.h"
#else ifdef _SERVER_BUILD
#include "..\Tanks-War-Server\server.h"
#endif
#include <memory>

class Graphics;
class Input;
class Timer;
class Texture;
class Image;
class Audio;
class Interface;
class Map;

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
	virtual void update() = 0;
	virtual void render() = 0;
	
	Graphics* getGraphics()	const { return m_pGraphics; }
	Input* getInput()	const { return m_pInput; }
	Audio* getAudio()	const { return m_pAudio; }
	TextureManger* getTextureManger() const { return m_pTextureManger; }
	Map* getMap()	const { return m_pMap; }
	Timer* getTimer()	const { return m_pTimer; }
#ifdef _CLIENT_BUILD
	Client* getClient() const { return m_pClient; }
#else ifdef _SERVER_BUILD
	Server* getServer() const { return m_pServer; }
#endif
	HWND getHwnd() const { return m_hwnd; }

protected:

	Graphics* m_pGraphics;
	Input* m_pInput;
	Audio* m_pAudio;
	TextureManger* m_pTextureManger;
	Interface* m_pInterface;
	Map* m_pMap;
	Timer* m_pTimer;
#ifdef _CLIENT_BUILD
	Client* m_pClient;
#else ifdef _SERVER_BUILD
	Server* m_pServer;
#endif
	HWND m_hwnd;
	float m_timeDeltaMillsec;
	float m_fps;
	Image m_logo;
};

#endif
