#include "TanksWar.h"
#include "player.h"
#include "fileio.h"
#include "texturemanger.h"

 //#define _CLIENT

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
	m_client.initialize(m_pMap);
#ifndef _CLIENT
	/*tank.initialize(&map, m_pTextureManger, m_pTextureManger->getTexture(1), m_pGraphics);
	tank.inputInitialize(m_pInput, W_KEY, S_KEY, D_KEY, A_KEY, T_KEY);
//	tank.setX(500).setY(500);
	Space space = map.getFreeSpace();
	tank.setX(space.x1).setY(space.y1);
	*/
#else
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
#ifndef _CLIENT
	//tank.update(0);
#else
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
#endif
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

	
//	map.draw();
#ifndef _CLIENT
//	tank.draw();
#else 
	player0.draw();
	for (int i = 0; i < m_players; i++)
		if (i == player0.getPlayerId())
			player0.draw();
		else
			m_pPlayer[i].draw();
#endif
} 

void TanksWar::onResetDevice()
{
 
}

void TanksWar::onLostDevice()
{
 
}
