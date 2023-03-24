#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "unit.h"
#include "tank.h"
#include "map.h"
#include "player.h"
#include "remoteplayer.h"
#include "client.h"

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void communicate();
	void update();
	void collision();
	void render();
	void onLostDevice();
	void onResetDevice();

private:
	
	uint8_t m_players;
	char* m_pServerIP;
	Client m_client;
};


#endif