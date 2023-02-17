#ifndef _TANKSWAR_H
#define _TANKSWAR_H

#include "game.h"
#include "unit.h"
#include "tank.h"
#include "map.h"
enum GTEXTURES {
	TEXTURE_PLAYERTANK,
	TEXTURE_EXPLOSION,
	TEXTURE_BM0,
	TEXTURE_BM1,
	TEXTURE_FIRE,
	TEXTURE_FIRE_EXPLOSION,
	TEXTURES
};
#define IMAGES 1
#define IMAGE_EXPLOSION 0

static std::string file[] = { "Assets\\Texture\\player-tank.png",
"Assets\\Texture\\explosion-animation.png" ,
 "Assets\\Texture\\bm0.jpg", "Assets\\Texture\\bm1.jpg","Assets\\Texture\\bullet_6.png", "Assets\\Texture\\explosion-fire-3.png" };

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND hWnd, bool fullscreen);
	void update();
	void collision();
	void render();
	void onLostDevice();
	void onResetDevice();

private:

	TextureManger* texture;
	Image* image;
	Tank* tank;
	Map map;

};


#endif