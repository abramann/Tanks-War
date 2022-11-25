#ifndef _TANKSWARFARE_H
#define _TANKSWARFARE_H
#include "game.h"
#include "unit.h"

constexpr auto FILE_GROUND = "Data//Texture//stone_floor.png";
constexpr auto FILE_FIRE_SIMPLE = "Data//Texture//Fire//1.png";
constexpr auto FILE_E100 = "Data//Texture//tanks//e-100.png";

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

	TextureManger *groundTex,*unitTex;
	Image ground;
	Unit unit;
};


#endif