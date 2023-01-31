#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "unit.h"
#include "tank.h"
#include "map.h"

#define TEXTURES			    6
#define TEXTURE_GROUND		    0
#define TEXTURE_PLAYERTANK	    1
#define TEXTURE_EXPLOSION	    2
#define TEXTURE_GROUNDSMEL		3
#define TEXTURE_WALL			4
#define TEXTURE_MAP				5
#define IMAGES 3
#define IMAGE_GROUND 0
#define IMAGE_EXPLOSION 1
#define IMAGE_MAP			2
#define UNITS 1
#define UNIT_TANK 0

static std::string file[] = { "Data\\Texture\\ground.jpg","Data\\Texture\\Tank\\player-tank.png",
"Data\\Texture\\Tank\\explosion_animation.png" ,"Data\\Texture\\ground.jpg",
"Data\\Texture\\wall.jpg", "Data\\Texture\\bitmap.png" };

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
	Map map;

};


#endif