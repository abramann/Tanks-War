// tankswar.h
// Author: abramann

#pragma once

#include "game.h"
#include "tank.h"
#include "map.h"
#include "client.h"

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void update();
	void render();
	void onLostDevice();
	void onResetDevice();

private:

	uint8_t m_players;
	char* m_pServerIP;
};