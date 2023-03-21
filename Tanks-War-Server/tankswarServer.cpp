#include "tankswarserver.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWarServer::TanksWarServer()
{
	m_players = 2;
	m_player = new RemotePlayer[m_players];
}

TanksWarServer::~TanksWarServer()
{
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND _hWnd)
{
	Game::initialize(hInstance, _hWnd);
	m_server.initialize();
	/*
	m_pAudio->playCue("Theme");
	m_server.initialize(m_players);
	
	m_pState = new PlayerState[m_players];
	
	for (int i = 0; i < m_players; i++)
	{
		m_player[i].initialize(&map, &tm, m_pGraphics);
		Space space = map.getFreeSpace();
		m_player[i].setX(space.x1).setY(space.y1);
		map.Add_Object(&m_player[i]);
		m_pState[i] = m_player[i].getPlayerState();
		m_pState[i].id = i;
	}

	m_server.send(m_pState, sizeof(PlayerState)*m_players);
	*/
}

void TanksWarServer::collision()
{
}

void TanksWarServer::update()
{
	return;
	PlayerToServer toServer;
	PlayerID id = m_server.recv(&toServer);
	if (id != INVALID_ID)
	{
		m_player[id].applyPlayerToServer(toServer, m_timeDelta);
		m_pState[id] = m_player[id].getPlayerState();
		m_pState[id].id = id;
		m_server.send(m_pState, sizeof(PlayerState)*m_players);

		for (int i = 0; i < m_players; i++)
			m_player[i].update(m_timeDelta);
	}
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
	for (int i = 0; i < m_players; i++)
		m_player[i].draw();
} 

void TanksWarServer::onResetDevice()
{
 
}

void TanksWarServer::onLostDevice()
{
 
}
