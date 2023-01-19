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
	image[IMAGE_GROUND].initialize(NULL, NULL, NULL, NULL, false, NULL, &texture[TEXTURE_GROUND], graphics);

	unit[UNIT_TANK].initialize(60, 60, 1, 1, false, 0, 100, 0.5f, 0, &texture[TEXTURE_PLAYERTANK], graphics);
	unit[UNIT_TANK].inputInitialize(input, W_KEY, S_KEY, D_KEY, A_KEY);
	unit[UNIT_TANK].setX(300);
	unit[UNIT_TANK].setY(300);
}


void TanksWar::collision()
{

}

void TanksWar::update()
{
	image[IMAGE_GROUND].update(timeDelta);
	unit[UNIT_TANK].update(timeDelta);
}

void TanksWar::render()
{
	image[IMAGE_GROUND].draw();
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
