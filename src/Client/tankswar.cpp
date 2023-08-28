// tankswar.cpp
// Author: abramann

#include "TanksWar.h"
#include "..\common\interface.h"
#include "..\common\input.h"
#include "..\common\fileio.h"
#include "..\common\data.h"
#include "..\common\serverplayer.h"

#define TEST_NO_SERVER_INTERFACE
#ifdef TEST_NO_SERVER_INTERFACE
#include "..\common\input.h"
#include "..\common\camera.h"
#include "..\common\tank.h"
#include "..\common\texturemanger.h"
Image image2;
Tank tank2;
#endif

TanksWar::TanksWar() : m_status(clientNS::CLIENT_UNCONNECTED)
{

	m_pClient = std::make_shared<Client>();
	m_pRData = m_pClient->getReceiveBuffer();
	m_pSData = m_pClient->getSendBuffer();

	m_pCpsJoin = (CpsJoin*)m_pSData;
	m_pCpsHeartbeat = (CpsHeartbeat*)m_pSData;
	m_pCpsDisconnect = (CpsDisconnect*)m_pSData;
	m_pSpsJoin = (SpsJoin*)m_pRData;
}

TanksWar::~TanksWar()
{
}

void TanksWar::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
#ifdef TEST_NO_SERVER_INTERFACE
	m_pMap->load("Nova");
	tank2.initialize(m_pTextureManger->getTexture("player-tank"), this);
	tank2.setPosition(V3(330, 300, 0));
#else
	m_pClient->initialize(this);
	m_pInterface->initialize(this);
	m_clientInfo = FileIO::readClientInfo();
#endif
}

void TanksWar::update()
{
#ifdef TEST_NO_SERVER_INTERFACE
	if (m_pInput->isKeyDown(inputNS::W_KEY))
		tank2.executeForward();
	if (m_pInput->isKeyDown(inputNS::S_KEY))
		tank2.executeBack();
	if (m_pInput->isKeyPressed(inputNS::D_KEY))
		tank2.executeRight();
	if (m_pInput->isKeyPressed(inputNS::A_KEY))
		tank2.executeLeft();
	if (m_pInput->isKeyDown(inputNS::E_KEY))
		tank2.executeAttack();
	if (GetAsyncKeyState('Q'))
		tank2.damage(100);

	tank2.update();
	m_pGraphics->getCamera()->update(tank2.getPosition());

	return;
#endif
}

void TanksWar::render()
{
#ifdef TEST_NO_SERVER_INTERFACE
	m_pMap->draw();
	tank2.draw();
	return;
#endif
	m_pInterface->render();
}

void TanksWar::communicate()
{
}

bool TanksWar::connect()
{
	if (!m_pClient->connect(m_clientInfo.serverIP, m_clientInfo.serverPort))
		return false;

	// Send join request packet
	m_pCpsJoin->packetType = PACKET_CLIENT_JOIN;
	strcpy(m_pCpsJoin->name, m_clientInfo.name);
	m_pClient->send<CpsJoin>();

	// Receive map and clients data packet
	if (m_pClient->recv(2000) && m_pSpsJoin->packetType == PACKET_CLIENT_JOIN)
	{
		m_id = m_pSpsJoin->id;
		recvGameProperties();
		m_status = clientNS::CLIENT_CONNECTED;
		return true;
	}
	
	disconnect();
	return false;
}

void TanksWar::disconnect()
{
	m_pClient->disconnect();
	m_pMap->clear();
	m_pRemoteClient.clear();
}

void TanksWar::updateClientInfo()
{
	FileIO::createClientInfo(&m_clientInfo);
}

void TanksWar::recvGameProperties()
{
	strcpy(m_map, m_pSpsJoin->map);
	m_pMap->load(m_map);
	m_clients = m_pSpsJoin->clients;
	auto& pServerClient = m_pSpsJoin->clientGameState;
	for (int i = 0; i < m_clients; i++)
	{
		if (pServerClient[i].id != m_id)
		{
			auto pClient = std::make_shared<RemoteClient>(pServerClient[i].id, pServerClient[i].name, this);
			m_pRemoteClient.push_back(pClient);
		}
	}
}

void TanksWar::setServerPort(Port port)
{
	m_clientInfo.serverPort = port;
	updateClientInfo();
}