#include "tanksWarfare.h"

TanksWarfare::TanksWarfare()
{
	groundTex = new TextureManger;
}
TanksWarfare::~TanksWarfare()
{
	releaseAll();
}
void TanksWarfare::initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen)
{
	Game::initialize(hInstance, _hWnd, _fullscreen);
	audio->playCue("Theme");
	if (!groundTex->initialize(FILE_GROUND, COLOR_WHITE, graphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize /Media/Texture/Stone_Floor.png");
	ground.initialize(NULL, NULL, NULL, NULL, false, 1, 1, 1, 0, groundTex, graphics);
}
void TanksWarfare::collision()
{
}
void TanksWarfare::update()
{
}
void TanksWarfare::render()
{
	ground.draw();
}
void TanksWarfare::resetAll()
{
	groundTex->onResetDevice();
	ground.setTexture(groundTex);
}
void TanksWarfare::releaseAll()
{
	groundTex->onLostDevice();
}