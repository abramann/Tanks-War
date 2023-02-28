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

void TanksWar::initialize(HINSTANCE hInstance, HWND _hWnd)
{
	Game::initialize(hInstance, _hWnd);
	
	tm.initialize(m_pGraphics);
	m_pAudio->playCue("Theme");
	map.initialize("Assets\\map.txt", tm.getTiledMapTexture(), m_pGraphics);
	tank1.initialize(&map, &tm, tm.getTexture(1), m_pGraphics);
	tank2.initialize(&map, &tm, tm.getTexture(1), m_pGraphics);
	tank1.setX(500).setY(300);
	tank2.setX(500).setY(500);

	m_server.initialize();
	PlayerInfo p0Info,p1Info;


	ServerToPlayer stp;
	stp.p0 = tank1.getPlayerState();
	stp.p1 = tank2.getPlayerState();
	m_server.send<ServerToPlayer>(&stp);


}

void TanksWar::collision()
{

}

void TanksWar::update()
{
	PlayerToServer pts = { 0 };
	int resp = m_server.recv<PlayerToServer>(&pts);
	if ( resp != NET_NORESPONSE)
	{
		if(resp==0)
			executeC1(pts);
		if(resp==1)
			executeC2(pts);
		ServerToPlayer stp;
		stp.p0 = tank1.getPlayerState();
		stp.p1 = tank2.getPlayerState();
		m_server.send<ServerToPlayer>(&stp);
	}
	tank1.update(0);
	tank2.update(0);
}

void TanksWar::render()
{
	map.draw();
	tank1.draw();
	tank2.draw();
} 

void TanksWar::onResetDevice()
{
 
}

void TanksWar::executeC1(PlayerToServer pts)
{
	if (pts.forward)
		tank1.executeForward(2);
	if(pts.back)
		tank1.executeBack(2);
	if (pts.left)
		tank1.executeLeft(2);
	if (pts.right)
		tank1.executeRight(2);
}

void TanksWar::executeC2(PlayerToServer pts)
{
	if (pts.forward)
		tank2.executeForward(2);
	if (pts.back)
		tank2.executeBack(2);
	if (pts.left)
		tank2.executeLeft(2);
	if (pts.right)
		tank2.executeRight(2);
}

void TanksWar::onLostDevice()
{
 
}
