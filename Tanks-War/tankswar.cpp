// tankswar.cpp
// Author: abramann

#include "TanksWar.h"
#include "interface.h"

#define TEST_NO_SERVER_INTERFACE
#ifdef TEST_NO_SERVER_INTERFACE
#include "input.h"
#include "camera.h"
#include "tank.h"
#include "texturemanger.h"
Image image2;
Tank tank2;
#endif

TanksWar::TanksWar()
{
	m_pClient = std::make_shared<Client>();
}

TanksWar::~TanksWar()
{
}

void TanksWar::initialize(HINSTANCE hInstance, HWND hWnd)
{
	Game::initialize(hInstance, hWnd);
#ifdef TEST_NO_SERVER_INTERFACE
	m_pMap->load("Old Empire");
	tank2.initialize(m_pTextureManger->getTexture(TEXTURE_PLAYER_TANK), this);
	tank2.setPosition(V3(330, 300, 0));
#else
	m_pClient->initialize(this);
	m_pInterface->initialize(m_pClient, this);
#endif
}

void TanksWar::update()
{
#ifdef TEST_NO_SERVER_INTERFACE
	if (m_pInput->isKeyDown(W_KEY))
		tank2.executeForward(0);
	if (m_pInput->isKeyDown(S_KEY))
		tank2.executeBack(0);
	if (m_pInput->isKeyPressed(D_KEY))
		tank2.executeRight(0);
	if (m_pInput->isKeyPressed(A_KEY))
		tank2.executeLeft(0);
	if (m_pInput->isKeyDown(E_KEY))
		tank2.executeAttack();
	if (GetAsyncKeyState('Q'))
		tank2.damage(100);

	tank2.update(m_timeDeltaMillsec);
	m_pGraphics->getCamera()->update(tank2.getPosition());

	return;
#endif
	m_pClient->update(m_timeDeltaMillsec);
	if (m_pClient->getState() == CLIENT_CONNECTED_PLAYING)
		if (m_pInput->isKeyDown(I_KEY))
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
	if (m_pClient->getState() == CLIENT_CONNECTED_PLAYING)
	{
		m_pMap->draw();
		ClientPlayer* clientPlayer = m_pClient->getClientPlayer();
		clientPlayer->draw();
		auto pClientData = m_pClient->getClientData();
		for (auto element : pClientData )
			element->serverPlayer.draw();
	}

	if (m_pClient->isConnected())
		return;
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
