#ifndef _TANKSWARSERVER_H
#define _TANKSWARSERVER_H

#include "game.h"
#include "unit.h"
#include "tank.h"
#include "map.h"
#include "remoteplayer.h"
#include "server.h"

class TanksWarServer : public Game
{
public:

	TanksWarServer();
	~TanksWarServer();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void update();
	void collision();
	void render();
	void onLostDevice();
	void onResetDevice();

private:

//	PlayerState* m_pState;
	TextureManger tm;
	Map map;
//	ServerToPlayer m_toPlayer1,m_toPlayer2;
//	PlayerToServer m_toServer;
//	RemotePlayer* m_player;
	Server m_server;
	uint8_t m_players;

};


#endif