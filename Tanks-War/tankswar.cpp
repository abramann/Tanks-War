#include "TanksWar.h"
#include "fileio.h"
#include "tank2.h"
#include "map2.h"
#include "texturemanger.h"
#include "camera.h"

#define TEST_NO_SERVER_INTERFACE
#ifdef TEST_NO_SERVER_INTERFACE
Image2 image2;
Tank2 tank2;
#endif

TanksWar::TanksWar()
{
}

TanksWar::~TanksWar()
{
}

void TanksWar::initialize(HINSTANCE hInstance, HWND hWnd)
{
	Game::initialize(hInstance, hWnd);
#ifdef TEST_NO_SERVER_INTERFACE
	m_pMap->load("Nova");
	tank2.initialize(m_pTextureManger->getTexture(TEXTURE_PLAYER_TANK), this);
	tank2.setPosition(V3(330, 300, 0));

#else
	m_client.initialize(m_pMap, m_pInput, m_pTextureManger, m_pAudio, m_pGraphics);
	m_pInterface->initialize(&m_client, m_pMap, m_pAudio, m_pGraphics);
#endif
}

void TanksWar::communicate()
{
}

void TanksWar::collision()
{

}

void TanksWar::update()
{
#ifdef TEST_NO_SERVER_INTERFACE
	tank2.update(m_timeDeltaMillsec);
	if (m_pInput->isKeyIn(W_KEY))
		tank2.executeForward(0);
	if (m_pInput->isKeyIn(S_KEY))
		tank2.executeBack(0);
	if (m_pInput->isKeyPressed(D_KEY))
		tank2.executeRight(0);
	if (m_pInput->isKeyPressed(A_KEY))
		tank2.executeLeft(0);
	if (m_pInput->isKeyIn(E_KEY))
		tank2.executeAttack();
	if (GetAsyncKeyState('Q'))
		tank2.damage(100);

	m_pGraphics->getCamera()->update(tank2.getPosition());

	return;
#endif
	m_client.update();
	if (m_client.getState() == CLIENT_CONNECTED_PLAYING)
		if (m_pInput->isKeyIn(I_KEY))
			if (m_pInterface->m_menu == MULTIPLAYER_MENU)
				m_pInterface->m_menu = PLAYING_MENU;
			else if (m_pInterface->m_menu == PLAYING_MENU)
				m_pInterface->m_menu = MULTIPLAYER_MENU;
				
}

void TanksWar::render()
{
#ifdef TEST_NO_SERVER_INTERFACE
	m_pMap->draw();
	tank2.draw();
	return;
#endif
	if (m_client.getState() == CLIENT_CONNECTED_PLAYING)
	{
		m_pMap->draw();

		for (auto clientData : m_client.getClientData())
			clientData.playerTank.draw();
	}
	switch (m_pInterface->m_menu)
	{
	case MAIN_MENU:
		m_pInterface->mainMenu();
		break;
	case MULTIPLAYER_MENU:
		m_pInterface->multiplayerMenu();
		break;
	case SETTING_MENU:
		m_pInterface->settingMenu();
		break;
	case QUIT_MENU:
		PostQuitMessage(0);
	default:
		break;
	}

	
} 

void TanksWar::onResetDevice()
{
 
}

void TanksWar::onLostDevice()
{
 
}
