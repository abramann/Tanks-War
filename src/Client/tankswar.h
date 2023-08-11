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
	void onLostDevice();
	void onResetDevice();

private:

	uint8_t m_players;
	char* m_pServerIP;
};