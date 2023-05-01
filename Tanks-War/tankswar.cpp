#include "TanksWar.h"
#include "fileio.h"
#include "tank2.h"
#include "map2.h"

#define TEST_NO_SERVER
#ifdef TEST_NO_SERVER
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
#ifdef TEST_NO_SERVER
	m_pMap->load("Nova");
	tank2.initialize(m_pTextureManger->getTexture(2), this);
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
#ifdef TEST_NO_SERVER
	tank2.update(m_timeDeltaMillsec);
//	image2.update(m_timeDeltaMillsec);
	if (GetAsyncKeyState('Y'))
		tank2.executeForward(0);
	if (GetAsyncKeyState('H'))
		tank2.executeBack(0);
	if (GetAsyncKeyState('J'))
		tank2.executeRight(0);
	if (GetAsyncKeyState('G'))
		tank2.executeLeft(0);
	if (GetAsyncKeyState('K'))
		tank2.executeAttack();
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
#ifdef TEST_NO_SERVER
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
