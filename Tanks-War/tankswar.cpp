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
	for (auto i = 0; i < TEXTURES; i++)
	{
		if (!texture[i].initialize(file[i].c_str(), COLOR_WHITE, m_pGraphics))
			throw GameError(gameErrorNS::FATAL_ERROR, "Could not initialize " + file[i]);
	}

	m_pAudio->playCue("Theme");
	
	image[IMAGE_EXPLOSION].initialize(128, 128, 8, 7, true, 200, &texture[TEXTURE_EXPLOSION], m_pGraphics);
	tank.initialize(0, 0, 9.0f, 5.5f, &image[IMAGE_EXPLOSION], &map, &texture[TEXTURE_PLAYERTANK], m_pGraphics);
	tank.inputInitialize(m_pInput, W_KEY, S_KEY, D_KEY, A_KEY, K_KEY, &texture[TEXTURE_EXPLOSION]);
	tank.setX(GAME_WIDTH / 2).setY(GAME_HEIGHT / 2).setFilterColor(COLOR_XRGB( 255, 100, 100));
	map.initialize("Assets\\map.txt", &texture[TEXTURE_BM0], m_pGraphics);
	tank.setScalling(1.0f);

}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	tank.update(m_timeDelta);
}

void TanksWar::render()
{
	map.draw();
	tank.draw();
}

void TanksWar::onResetDevice()
{
	for (auto i = 0; i < TEXTURES; i++)
		texture[i].onResetDevice();
}

void TanksWar::onLostDevice()
{
	for (auto i = 0; i < TEXTURES; i++)
		texture[i].onLostDevice();
}
