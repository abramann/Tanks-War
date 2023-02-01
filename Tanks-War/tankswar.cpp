#include "TanksWar.h"

TanksWar::TanksWar()
{
	texture = new TextureManger[TEXTURES];
	image = new Image[IMAGES];
	unit = new Unit[UNITS];
}

TanksWar::~TanksWar()
{
	SAFE_DELETE_ARRAY(texture);
	SAFE_DELETE_ARRAY(image);
	SAFE_DELETE_ARRAY(unit);
}

void TanksWar::initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen)
{
	Game::initialize(hInstance, _hWnd, _fullscreen);
	for (int i = 0; i < TEXTURES; i++)
	{
		if (!texture[i].initialize(file[i].c_str(), COLOR_WHITE, graphics))
			throw GameError(gameErrorNS::FATAL_ERROR, "Could not initialize " + file[i]);
	}

	audio->playCue("Theme");
	
	image[IMAGE_EXPLOSION].initialize(128, 128, 8, 7, true, 200, &texture[TEXTURE_EXPLOSION], graphics);
	unit[UNIT_TANK].initialize(60, 60, 1, 1, false, 0, 100, 0.5f, &image[IMAGE_EXPLOSION], &texture[TEXTURE_PLAYERTANK],&map, graphics);
	unit[UNIT_TANK].inputInitialize(input, W_KEY, S_KEY, D_KEY, A_KEY);
	unit[UNIT_TANK].setX(500)->setY(500);
	map.initialize("map.txt", 2, &texture[TEXTURE_MAP], graphics);
}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	image[IMAGE_MAP].update(timeDelta);
	if (input->isKeyIn(O_KEY))
		unit[UNIT_TANK].setHealth(0);

	unit[UNIT_TANK].update(timeDelta);
}

void TanksWar::render()
{
	map.draw();
	unit[UNIT_TANK].setScalling(1.0f);
	unit[UNIT_TANK].draw();
}

void TanksWar::onResetDevice()
{
	for (int i = 0; i < TEXTURES; i++)
	{
		texture[i].onResetDevice();
	}
	image[IMAGE_GROUND].setTexture(&texture[TEXTURE_GROUND]);
}

void TanksWar::onLostDevice()
{
	for (int i = 0; i < TEXTURES; i++)
	{
		texture[i].onLostDevice();
	}
}
