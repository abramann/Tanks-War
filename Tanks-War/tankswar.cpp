#include "TanksWar.h"
#include "player.h"
Image point;
TanksWar::TanksWar()
{
	image = new Image[IMAGES];
	texture = new TextureManger[TEXTURES];
	tank = new Tank[2];
}

TanksWar::~TanksWar()
{
	SAFE_DELETE_ARRAY(texture);
	SAFE_DELETE_ARRAY(image);
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
	tank[0].setX(300).setY(500);
	tank[0].inputInitialize(m_pInput, W_KEY, S_KEY, D_KEY, A_KEY, Q_KEY, &texture[TEXTURE_FIRE]);
	map.initialize("Assets\\map.txt", &texture[TEXTURE_BM0], m_pGraphics);
	tank[1].initialize(0, 0, 100, 8, &map, &texture[TEXTURE_EXPLOSION], &texture[TEXTURE_PLAYERTANK], m_pGraphics);
//	tank[1].inputInitialize(m_pInput, V_KEY, J_KEY, H_KEY, G_KEY, F_KEY, &texture[TEXTURE_FIRE]);
	tank[1].setX(300).setY(300);
	point.initialize(30, 30,1 , 1, 0, 0, &texture[TEXTURE_FIRE_EXPLOSION], m_pGraphics);
	point.setScalling(0.5f);
	point.setFrameRect(1, 1, 0);
	map.Add_Object(&tank[1]);
	
}

void TanksWar::collision()
{

}
void TanksWar::update()
{
	tank[1].update(m_timeDelta);
	tank[0].update(m_timeDelta);
	point.update(m_timeDelta);
	 
	point.setLookTo(tank[0]);
}

void TanksWar::render()
{
	map.draw();
	tank[1].draw();
	tank[0].draw();
	point.draw();
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
