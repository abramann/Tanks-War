#include "TanksWar.h"
#include "player.h"
#include "fileio.h"
#include "texturemanger.h"

Image point;
TanksWar::TanksWar()
{
	image = new Image[IMAGES];
	tank = new Tank[2];
	texture = new Texture[TEXTURES];
}

TanksWar::~TanksWar()
{
	SAFE_DELETE_ARRAY(texture);
	SAFE_DELETE_ARRAY(image);
}

void TanksWar::initialize(HINSTANCE hInstance, HWND _hWnd)
{
	Game::initialize(hInstance, _hWnd);
	
	
	tm.initialize(m_pGraphics);

//	image[1].initialize(&tm, m_pGraphics);
	m_pAudio->playCue("Theme");
	tank->initialize(&map, &tm, tm.getTexture(1), m_pGraphics);
	tank->inputInitialize(m_pInput, W_KEY, S_KEY, D_KEY, A_KEY, E_KEY);
	tank->setX(600).setY(300);
	tank[1].initialize(&map, &tm, tm.getTexture(1), m_pGraphics);
	tank[1].setX(400).setY(300);
	map.initialize("Assets\\map.txt", tm.getTiledMapTexture(), m_pGraphics);
 
	point.initialize(30, 30,1 , 1, 0, 0, &texture[TEXTURE_FIRE_EXPLOSION], m_pGraphics);
	point.setScalling(0.5f);
	point.setFrameRect(1, 1, 0);
	map.Add_Object(&tank[0]);
	map.Add_Object(&tank[1]);
}

void TanksWar::collision()
{

}
void TanksWar::update()
{
	tank[0].update(m_timeDelta);
	tank[1].update(m_timeDelta);
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
