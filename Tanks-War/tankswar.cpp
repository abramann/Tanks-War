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

void TanksWar::initialize(HINSTANCE hInstance, HWND hWnd)
{
	Game::initialize(hInstance, hWnd);
	
	tm.initialize(m_pGraphics);
	m_pAudio->playCue("Theme");
	map.initialize("Assets\\map.txt", tm.getTiledMapTexture(), m_pGraphics);
	player0.initialize(&map, &tm, m_pInput, m_pGraphics);
	player1.initialize(&map, &tm, m_pGraphics);

	m_client.initialize(m_pServerIP);
	m_client.send("in");
	Sleep(50);

	PlayerInfo playerInfo;
	m_client.recv<PlayerInfo>(&playerInfo, true);
	player0.setPlayerInfo(playerInfo);

	PlayerState playerState;
	m_client.recv<PlayerState>(&playerState, true);
	player0.update(playerState);
	m_client.recv<PlayerState>(&playerState, true);
	player1.update(playerState);
}

void TanksWar::communicate()
{
	if (player0.m_event)
		m_client.send<PlayerToServer>(&player0.getPlayerToServer());
	
	ServerToPlayer svp = { 0 };
	m_client.recv<ServerToPlayer>(&svp);
	player0.update(svp);
	m_client.recv<ServerToPlayer>(&svp);
	player1.update(svp);
}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	/*PlayerState playerState;
	m_client.recv<PlayerState>(&playerState);
	player0.update(0, playerState);
	m_client.recv<PlayerState>(&playerState);
	player1.update(0, playerState);*/
}

void TanksWar::render()
{
	map.draw();
	player0.draw();
	player1.draw();
} 

void TanksWar::onResetDevice()
{
 
}

void TanksWar::onLostDevice()
{
 
}
