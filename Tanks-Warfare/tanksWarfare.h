#ifndef _TANKSWARFARE_H
#define _TANKSWARFARE_H
#include "game.h"
#include "tank.h"
#include "e100.h"

#define FILE_GROUND "Data//Texture//Stone_Floor.png"
#define FILE_E100 "Data//Texture//tanks//E-100.png"

class TanksWarfare : public Game
{
public:

	TanksWarfare();
	~TanksWarfare();
	void initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen);
	void update();
	void collision();
	void render();
	void resetAll();
	void onLostDevice();
	void onResetDevice();

private:

	TextureManger *groundTex;
	Image ground;
};


#endif