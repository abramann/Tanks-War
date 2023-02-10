#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "unit.h"
#include "tank.h"
#include "map.h"

#define TEXTURES			    4
#define TEXTURE_PLAYERTANK	    0
#define TEXTURE_EXPLOSION	    1
#define TEXTURE_BM0				2
#define TEXTURE_BM1 			3
#define IMAGES 1
#define IMAGE_EXPLOSION 0
#define UNITS 1
#define UNIT_TANK 0

static std::string file[] = { "Data\\Texture\\Tank\\player-tank.png",
"Data\\Texture\\Tank\\explosion_animation.png" ,
 "Data\\Texture\\bm0.jpg", "Data\\Texture\\bm1.jpg" };

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen);
	void update();
	void collision();
	void render();
	void onLostDevice();
	void onResetDevice();

private:

	TextureManger* texture;
	Image* image;
	Unit* unit;
	Tank tank;
	Map map;

};


#endif