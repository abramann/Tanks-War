#ifndef _TANKSWARSERVER_H
#define _TANKSWARSERVER_H

#include "game.h"
#include "server.h"

class TanksWarServer : public Game
{
public:

	TanksWarServer();
	~TanksWarServer();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void update();
	void render();

private:

};


#endif