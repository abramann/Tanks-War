// tankswarserver.cpp
// Author: abramann

#include "tankswarserver.h"
#include "fileio.h"
#include "texturemanger.h"
#include "interface.h"

TanksWarServer::TanksWarServer()
{
	m_pServer = std::make_shared<Server>();
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

void TanksWarServer::update()
{
	m_pServer->update();
}

void TanksWarServer::render()
{
	m_pInterface->show();
	if(m_pServer->getState() == SERVER_RUNNING_HANDLING)
	{
		// m_pMap->draw();
	//	for (auto pClientData : m_pServer->getClientData())
		//	pClientData->draw();
	}	
} 
