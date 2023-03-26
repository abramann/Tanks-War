#include "TanksWar.h"
#include "player.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWar::TanksWar()
{
}

TanksWar::~TanksWar()
{
}

void TanksWar::initialize(HINSTANCE hInstance, HWND hWnd)
{
	Game::initialize(hInstance, hWnd);
	
//	m_pAudio->playCue("Theme");
	m_client.initialize(m_pMap);

}

void TanksWar::communicate()
{
}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	m_client.update();
}

void TanksWar::render()
{
	switch (m_pInterface->m_menu)
	{
	case MAIN_MENU:
		m_pInterface->mainMenu();
		break;
	case MULTIPLAYER_MENU:
		m_pInterface->multiplayerMenu(m_client);
		break;
	case SETTING_MENU:
		m_pInterface->settingMenu(*m_pGraphics, *m_pAudio);
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
