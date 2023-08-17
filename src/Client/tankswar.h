// tankswar.h
// Author: abramann

#pragma once

#include "..\common\game.h"
#include "..\common\tank.h"
#include "..\common\map.h"
#include "client.h"

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void update();
	void render();
	char* getServerIP() { return m_clientInfo.serverIP; }
	char* getPlayerName() { return m_clientInfo.name; }
	Port getServerPort() { return m_clientInfo.serverPort; }
	void updateClientInfo();
	void setServerPort(Port port);

private:

	ClientInfo m_clientInfo;
	uint8_t m_players;
};