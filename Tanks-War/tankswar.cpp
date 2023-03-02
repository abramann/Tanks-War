#include "TanksWar.h"
#include "player.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWar::TanksWar()
{
	m_pServerIP = "127.0.0.1";
}

TanksWar::~TanksWar()
{
}
Tank tank;
void TanksWar::initialize(HINSTANCE hInstance, HWND hWnd)
{
	Game::initialize(hInstance, hWnd);
	
	m_pAudio->playCue("Theme");
	map.initialize("Assets\\map.txt", m_pTextureManger->getTiledMapTexture(), m_pGraphics);
	tank.initialize(&map, m_pTextureManger, m_pTextureManger->getTexture(1), m_pGraphics);
	tank.inputInitialize(m_pInput, W_KEY, S_KEY, D_KEY, A_KEY, T_KEY);
	tank.setX(500).setY(500);

	return;
	char playerName[20];
	sprintf(playerName, "Player %d", _rand(100));
	m_client.initialize(m_pServerIP, playerName, m_players);

	m_pPlayer = new RemotePlayer[m_players];
	PlayerInfo* pPlayerInfo = new PlayerInfo[m_players];
	m_pPlayerState = new PlayerState[m_players];

	m_client.recv(pPlayerInfo, true);
	m_client.recv(m_pPlayerState, true);

	for (int i = 0; i < m_players; i++)
	{
		if (strcmp(playerName, pPlayerInfo[i].name) == 0)
		{
			player0.initialize(&map, m_pTextureManger, m_pInput, m_pGraphics);
			player0.setPlayerInfo(pPlayerInfo[i]);
			player0.update(m_pPlayerState[i]);
			continue;
		}

		m_pPlayer[i].initialize(pPlayerInfo[i], &map, m_pTextureManger, m_pGraphics);
		m_pPlayer[i].update(m_pPlayerState[i]);
	}
	
}

void TanksWar::communicate()
{
	
}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	tank.update(0);
	return;
	if (m_client.recv(m_pPlayerState) == NET_RESPONSE)
	{
		for (int i = 0; i < m_players; i++)
		{
			if (m_pPlayerState[i].id == player0.getPlayerId())
			{
				player0.update(m_pPlayerState[i]);
				continue;
			}

			m_pPlayer[i].update(m_pPlayerState[i]);
		}

		return;
	}
	

	player0.update();
	if (player0.m_event)
	{
		PlayerToServer pts = player0.getPlayerToServer();
		m_client.send<PlayerToServer>(&pts);
	}

	for (int i = 0; i < m_players; i++)
	{
		if (i == player0.getPlayerId())
		{
			player0.update();
			continue;
		}
		m_pPlayer[i].update();
	}
}

void TanksWar::render()
{
	map.draw();
	tank.draw();
	return;
//	player0.draw();
	for (int i = 0; i < m_players; i++)
		if (i == player0.getPlayerId())
			player0.draw();
		else
			m_pPlayer[i].draw();

} 

void TanksWar::onResetDevice()
{
 
}

void TanksWar::onLostDevice()
{
 
}
