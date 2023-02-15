#include "TanksWar.h"
#include "player.h"

TanksWar::TanksWar()
{
	image = new Image[IMAGES];
	texture = new TextureManger[TEXTURES];
	unit = new Unit[UNITS];
	tank = new Tank;
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
		if (!texture[i].initialize(file[i].c_str(), m_pGraphics))
			throw GameError(gameErrorNS::FATAL_ERROR, "Could not initialize " + file[i]);
	}
	
	m_pAudio->playCue("Theme");
	tank[0].initialize(0, 0, 100, 8, &map,&texture[TEXTURE_EXPLOSION], &texture[TEXTURE_PLAYERTANK],m_pGraphics);
	tank[0].setX(300).setY(300);
	tank[0].inputInitialize(m_pInput, W_KEY, S_KEY, D_KEY, A_KEY, Q_KEY, &texture[TEXTURE_FIRE]);
	//tank[0].setX(GAME_WIDTH / 2).setY(GAME_HEIGHT / 2).setFilterColor(COLOR_XRGB( 255, 100, 100));
	map.initialize("Assets\\map.txt", &texture[TEXTURE_BM0], m_pGraphics);
	 
}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	tank[0].update(m_timeDelta);
	if (m_pInput->isKeyIn(D_KEY))
		unit[0].setHealth(0);
	unit[0].update(m_timeDelta);
}

void TanksWar::render()
{
	map.draw();
	//unit[0].draw();
	tank[0].draw();
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
