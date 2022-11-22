#include "tanksWarfare.h"
TextDX t;
TanksWarfare::TanksWarfare()
{
	groundTex = new TextureManger;
}
TanksWarfare::~TanksWarfare()
{
	safeDelete(groundTex);
}
void TanksWarfare::initialize(HINSTANCE hInstance, HWND _hWnd, bool _fullscreen)
{
	Game::initialize(hInstance, _hWnd, _fullscreen);
	audio->playCue("Theme");
	if (!groundTex->initialize(FILE_GROUND, COLOR_WHITE, graphics))
		throw GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize /Media/Texture/Stone_Floor.png");
	ground.initialize(NULL, NULL, NULL, NULL, false, 1, 1, 1, 0, groundTex, graphics);
	t.initialize(30, false, false, "Arial", graphics);
	t.setColor(COLOR_WHITE);
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
	t.printf(30, 30, (std::string) "AAAAAAAAAAAA");
}
void TanksWarfare::onResetDevice()
{
	groundTex->onResetDevice();
	ground.setTexture(groundTex);
	t.onResetDevice();
}
void TanksWarfare::onLostDevice()
{
	groundTex->onLostDevice();
	t.onLostDevice();
}