// game.h
// Author: abramann
// Note this file is influenced by game.h from Chrles Kelly's Programming 2D Games Copyright (c) CC BY 3.0
// Note parts of this code are licensed under CC BY 3.0

// Programming 2D Games Copyright (c) 2011 by: Charles Kelly
// game.h

#pragma once

#include "types.h"
#ifdef _CLIENT_BUILD
#include "..\Client\client.h"
#else ifdef _SERVER_BUILD
#include "..\Server\server.h"
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
class Dx11Wrapper;

class Game
{
public:

	Game();
	~Game();
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void initialize(HINSTANCE hInstance, HWND hwnd);
	virtual void run();
	virtual void renderGame();
	virtual void updateGame();
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void communicate() = 0;
	virtual bool isOnline() const = 0;
	void onUpdateDisplay() const;
	void updateGameSettings() const;
	bool checkGameFiles() const;
	Resolution getResolution() const;
	void showLogo() const;
	Graphics* getGraphics()	const { return m_pGraphics.get(); }
	Input* getInput()	const { return m_pInput.get(); }
	Audio* getAudio()	const { return m_pAudio.get(); }
	TextureManger* getTextureManger() const { return m_pTextureManger.get(); }
	Map* getMap()	const { return m_pMap.get(); }
	Timer* getTimer()	const { return m_pTimer.get(); }
	Dx11Wrapper* getDx11Wrapper() const { return m_pDx11Wrapper.get(); }
#ifdef _CLIENT_BUILD
	Client* getClient() const { return m_pClient.get(); }
#else ifdef _SERVER_BUILD
	Server* getServer() const { return m_pServer.get(); }
#endif
	HWND getHwnd() const { return m_hwnd; }

protected:

	std::shared_ptr<Graphics> m_pGraphics;
	std::shared_ptr<Input> m_pInput;
	std::shared_ptr<Audio> m_pAudio;
	std::shared_ptr<TextureManger> m_pTextureManger;
	std::shared_ptr<Interface> m_pInterface;
	std::shared_ptr<Map> m_pMap;
	std::shared_ptr<Timer> m_pTimer;
	std::shared_ptr<Dx11Wrapper> m_pDx11Wrapper;
#ifdef _CLIENT_BUILD
	std::shared_ptr<Client> m_pClient;
#else ifdef _SERVER_BUILD
	std::shared_ptr<Server> m_pServer;
#endif
	HWND m_hwnd;
	float m_fps;
};
