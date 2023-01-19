#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "unit.h"
#include "tank.h"

#define TEXTURES			  3
#define TEXTURE_GROUND		  0
#define TEXTURE_PLAYERTANK	  1
#define TEXTURE_EXPLOSION	  2
#define IMAGES 2
#define IMAGE_GROUND 0
#define IMAGE_EXPLOSION 1
#define UNITS 1
#define UNIT_TANK 0

static std::string file[3] = { "Data\\Texture\\stone_floor.png","Data\\Texture\\Tank\\player-tank.png","Data\\Texture\\Tank\\explosion_animation.png" };

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
};


#endif