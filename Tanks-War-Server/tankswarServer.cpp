// tankswarserver.cpp
// Author: abramann

#include "tankswarserver.h"
#include "fileio.h"
#include "texturemanger.h"

TanksWarServer::TanksWarServer()
{
	m_pServer = new Server;
}

TanksWarServer::~TanksWarServer()
{
	safeDelete(m_pServer);
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
	m_pServer->initialize(this);
	m_pInterface->initialize(m_pServer, this);
}

void TanksWarServer::update()
{
	m_pServer->update(m_timeDeltaMillsec);
}

void TanksWarServer::render()
{
	m_pInterface->show();
	if(m_pServer->getState() == SERVER_RUNNING_HANDLING)
	{
		m_pMap->draw();
		for (auto pClientData : m_pServer->getClientData())
			pClientData->draw();
	}	
} 
