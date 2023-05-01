#include "tankswarserver.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWarServer::TanksWarServer()
{
}

TanksWarServer::~TanksWarServer()
{
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
	m_server.initialize(m_pMap, m_pTextureManger, m_pAudio, m_pGraphics);
	m_pInterface->initialize(&m_server, m_pMap, m_pAudio, m_pGraphics);
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
	m_pInterface->show();
//	m_pInterface->showFPS(m_fps);
	if (m_server.getState() == SERVER_RUNNING_HANDLING)
	{
		m_pMap->draw();
		for (auto clientData : m_server.getClientData())
			clientData.playerTank.draw();
	}

	
} 

void TanksWarServer::onResetDevice()
{
 
}

void TanksWarServer::onLostDevice()
{
 
}
