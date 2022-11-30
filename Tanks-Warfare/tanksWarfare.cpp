#include "tanksWarfare.h"

TanksWarfare::TanksWarfare()
{
	groundTex = new TextureManger;
	unitTex = new TextureManger;
}

TanksWarfare::~TanksWarfare()
{
	SAFE_DELETE(groundTex);
	SAFE_DELETE(unitTex);
}

void TanksWarfare::initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen)
{
	Game::initialize(hInstance, _hWnd, _fullscreen);
	audio->playCue("Theme");
	if (!groundTex->initialize(FILE_GROUND, COLOR_WHITE, graphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize /Media/Texture/Stone_Floor.png");
	if (!unitTex->initialize(FILE_E100, COLOR_WHITE, graphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize /Media/Texture/Fire/fire-simple.png");

	ground.initialize(0, 0, 0, 0, 0, 1, 1, 1, 0, groundTex, graphics);
	unit.initialize(96,800, 0, 0, 1, 1,3, 100, 2, unitTex, graphics);
	unit.inputInitialize(input, W_KEY, S_KEY, D_KEY, A_KEY);
	unit.audioInitialize("Tank_Forward", "Tank_Back", "Tanks_Idle", "Tank_Idle","Tank_Death");
	unit.setScalling(2.0f);
	unit.setX(300);
	unit.setY(300);
}


void TanksWarfare::collision()
{
	if (input->isKeyIn(O_KEY))
		unit.setHealth(0);
}

void TanksWarfare::update()
{
	ground.update(timeDelta);
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
