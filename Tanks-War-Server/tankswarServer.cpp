#include "tankswarserver.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWarServer::TanksWarServer()
{
	m_pServer = std::make_unique<Server>();
}

TanksWarServer::~TanksWarServer()
{
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
	m_pServer->initialize(this);
	m_pInterface->initialize(m_pServer.get(), this);
}

void TanksWarServer::collision()
{
}

void TanksWarServer::update()
{
	m_pServer->update();
}

void TanksWarServer::render()
{
	m_pInterface->show();
//	m_pInterface->showFPS(m_fps);
	if (m_pServer->getState() == SERVER_RUNNING_HANDLING)
	{
		m_pMap->draw();
		for (auto clientData : m_pServer->getClientData())
			clientData.serverPlayer.draw();
	}

	
} 
