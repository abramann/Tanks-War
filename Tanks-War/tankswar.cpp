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
	player0.setPlayerId(playerInfo.id);
	uint8_t id;
	if (playerInfo.id == 1)
		id = 0;
	else
		id = 1;
	player1.setPlayerId(id);

	ServerToPlayer stp = { 0 };
	m_client.recv<ServerToPlayer>(&stp, true);
	player0.update(stp);
	player1.update(stp);
}

void TanksWar::communicate()
{
	if (player0.m_event)
	{
		PlayerToServer pts = player0.getPlayerToServer();
		m_client.send<PlayerToServer>(&pts);
	}
	player0.update();	
	player1.update();

	ServerToPlayer svp;
	if (m_client.recv<ServerToPlayer>(&svp) == NET_RESPONSE)
	{
		player0.update(svp);
		player1.update(svp);
	}
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
