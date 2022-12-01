#ifndef _TANKSWARFARE_H
#define _TANKSWARFARE_H
#include "game.h"
#include "unit.h"
#include "tank.h"
constexpr auto FILE_GROUND = "Data\\Texture\\stone_floor.png";
constexpr auto FILE_FIRE_SIMPLE = "Data\\Texture\\Fire\\fire-simple.png";
constexpr auto FILE_E100 = "Data\\Texture\\tank\\e-100.png";
constexpr auto FILE_VK360H1 = "Data\\Texture\\Tank\\VK3601h.png";
constexpr auto FILE_PLAYERTANK = "Data\\Texture\\Tank\\player-tank.png";

class TanksWarfare : public Game
{
public:

	TanksWarfare();
	~TanksWarfare();
	void initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen);
	void update();
	void collision();
	void render();
	void onLostDevice();
	void onResetDevice();

private:

	TextureManger *groundTex,*unitTex,*fireTex;
	Image ground;
	Tank unit;
};


#endif