#include "tanksWarfare.h"

TanksWarfare::TanksWarfare()
{
	groundTex = new TextureManger;
	unitTex = new TextureManger;
}

TanksWarfare::~TanksWarfare()
{
	safeDelete(groundTex);
	safeDelete(unitTex);
}

void TanksWarfare::initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen)
{
	Game::initialize(hInstance, _hWnd, _fullscreen);
	audio->playCue("Theme");
	if (!groundTex->initialize(FILE_GROUND, COLOR_WHITE, graphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize /Media/Texture/Stone_Floor.png");

	ground.initialize(NULL, NULL, NULL, NULL, false, 1, 1, 1, 0, groundTex, graphics);
	unitTex->initialize(FILE_FIRE_SIMPLE, COLOR_WHITE, graphics);
	unit.initialize(0, 0, 0, 0, 1, 1, 1, 100, 2, unitTex, graphics);
	unit.inputInitialize(input, W_KEY, S_KEY, D_KEY, A_KEY);
	unit.audioInitialize("Tank_Forward", "Tank_Back", "Tanks_fast", "Tank_fast");
	unit.setScalling(2.0f);
	unit.setX(300);	unit.setY(300);
}

void TanksWarfare::collision()
{
}

void TanksWarfare::update()
{
	unit.update(timeDelta);
}

void TanksWarfare::render()
{
	ground.draw();
	unit.draw();
}

void TanksWarfare::onResetDevice()
{
	groundTex->onResetDevice();
	ground.setTexture(groundTex);
	unitTex->onResetDevice();
	unit.setTexture(unitTex);
}

void TanksWarfare::onLostDevice()
{
	groundTex->onLostDevice();
	unitTex->onLostDevice();
}
