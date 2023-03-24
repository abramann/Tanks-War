#include "tankswarserver.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWarServer::TanksWarServer()
{
}

TanksWarServer::~TanksWarServer()
{
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND _hWnd)
{
	Game::initialize(hInstance, _hWnd);
	m_server.initialize(m_pMap);
}

void TanksWarServer::collision()
{
}

void TanksWarServer::update()
{
	m_server.update();

	
}

void TanksWarServer::render()
{
	switch (m_pInterface->m_menu)
	{
	case MAIN_MENU:
		m_pInterface->mainMenu();
		break;
	case MULTIPLAYER_MENU:
		m_pInterface->multiplayerMenu(m_server, *m_pMap);
		break;
	case SETTING_MENU:
		m_pInterface->settingMenu(*m_pGraphics, *m_pAudio);
		break;
	case QUIT_MENU:
		PostQuitMessage(0);
	default:
		break;
	}

	return;
	map.draw();
	
} 

void TanksWarServer::onResetDevice()
{
 
}

void TanksWarServer::onLostDevice()
{
 
}
