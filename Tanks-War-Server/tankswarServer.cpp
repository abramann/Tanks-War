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
	
	tm.initialize(m_pGraphics);
	m_pAudio->playCue("Theme");
	m_server.initialize(m_players);

	map.initialize("Assets\\map.txt", tm.getTiledMapTexture(), m_pGraphics);
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
}

void TanksWarServer::collision()
{
}

void TanksWarServer::update()
{
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
