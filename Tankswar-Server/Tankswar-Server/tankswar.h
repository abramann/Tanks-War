#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "unit.h"
#include "tank.h"
#include "map.h"
#include "player.h"
#include "remoteplayer.h"
#include "server.h"


static std::string file[] = { "Assets\\Texture\\player-tank.png",
"Assets\\Texture\\explosion-animation.png" ,
 "Assets\\Texture\\bm0.jpg", "Assets\\Texture\\bm1.jpg","Assets\\Texture\\bullet_6.png", "Assets\\Texture\\explosion-fire-3.png", "Assets\\Texture\\fire-simple.png" };

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void update();
	void collision();
	void render();
	void onLostDevice();
	void onResetDevice();
	void executeC1(PlayerToServer pts);
	void executeC2(PlayerToServer pts);

private:

	TextureManger tm;
	Map map;
	ServerToPlayer m_toPlayer1,m_toPlayer2;
	PlayerToServer m_toServer;
	Tank tank1, tank2;
	Server m_server;

};


#endif