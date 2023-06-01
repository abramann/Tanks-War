// tankswar.h
// Author: abramann

#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "tank2.h"
#include "map2.h"
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


#endif