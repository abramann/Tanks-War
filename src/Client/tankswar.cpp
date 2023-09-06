// tankswar.cpp
// Author: abramann

#include "TanksWar.h"
#include "..\common\interface.h"
#include "..\common\input.h"
#include "..\common\fileio.h"
#include "..\common\data.h"
#include "..\common\serverplayer.h"
#include "..\common\timer.h"

//#define TEST_NO_SERVER_INTERFACE
#ifdef TEST_NO_SERVER_INTERFACE
#include "..\common\input.h"
#include "..\common\camera.h"
#include "..\common\tank.h"
#include "..\common\texturemanger.h"
#include "dx11wrapper.h"
Image image2;
Tank tank2;
#endif

TanksWar::TanksWar() : m_status(clientNS::CLIENT_UNCONNECTED)
{
	m_pClient = std::make_shared<Client>();
	m_pRData = m_pClient->getReceiveBuffer();
	m_pSData = m_pClient->getSendBuffer();
	m_pPacketType = (PacketType*)m_pRData;
	m_pCpsJoin = (CpsJoin*)m_pSData;
	m_pCpsHeartbeat = (CpsHeartbeat*)m_pSData;
	m_pCpsDisconnect = (CpsDisconnect*)m_pSData;
	m_pCpsPlayerAct = (CpsPlayerAct*)m_pSData;
	m_pSpsJoin = (SpsJoin*)m_pRData;
	m_pSpsClientInitialData = (SpsClientInitialData*)m_pRData;
	m_pSpsClientGameState = (SpsClientGameState*)m_pRData;
	m_pSpsDisconnect = (SpsDisconnect*)m_pRData;
	m_pSpsPlayerAct = (SpsPlayerAct*)m_pRData;
}

TanksWar::~TanksWar()
{
	if (m_status == clientNS::CLIENT_CONNECTED)
		disconnect();
}

void TanksWar::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
#ifdef TEST_NO_SERVER_INTERFACE
	m_pMap->load("Nova");
	tank2.initialize("player-tank", this);
	tank2.setPosition(V3(330, 300, 0));
#else
	m_pClient->initialize(this);
	m_pInterface->initialize(this);
	m_clientInfo = FileIO::readClientInfo();
	m_thisClient.initialize(0, this);
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
	if (GetAsyncKeyState('Y'))
		tank2.executeAnimateRepeat();

	tank2.update();
	m_pGraphics->getCamera()->update(tank2.getPosition());

	return;
#endif
}

void TanksWar::render()
{
#ifdef TEST_NO_SERVER_INTERFACE
	updateGame();
	m_pMap->draw();
	tank2.draw();
	return;
#else
	m_pInterface->render();
#endif
}

void TanksWar::renderScene()
{
	m_pMap->draw();
	m_thisClient.draw();
	for (auto& pRClient : m_pRemoteClient)
		pRClient->draw();
}

void TanksWar::updateScene()
{
	m_thisClient.update();
	for (auto& pRClient : m_pRemoteClient)
		pRClient->update();
}

void TanksWar::communicate()
{
	heartbeat();
	if (!m_pClient->recv())
		return;

	switch (*m_pPacketType)
	{
	case PACKET_CLIENT_GAME_STATE:
		applyUpdateClientGameState();
		break;

	case PACKET_DISCONNECT:
		applyDisconnect();
		break;

	case PACKET_SERVER_SHUTDOWN:
		disconnect();
		break;

	case PACKET_CLIENT_JOIN:
		m_pRemoteClient.push_back(std::make_shared<RemoteClient>(m_pSpsJoin->id, m_pSpsJoin->name, this));
		break;

	case PACKET_CLIENT_ACT:
		executeClientAct();
	
	default:
		break;
	}
}

bool TanksWar::connect()
{
	if (!m_pClient->connect(m_clientInfo.serverIP, m_clientInfo.serverPort))
		return false;

	// Send join request packet
	m_pCpsJoin->packetType = PACKET_CLIENT_JOIN;
	strcpy(m_pCpsJoin->name, m_clientInfo.name);
	m_pClient->send<CpsJoin>();

	// Receive map and clients data packets
	if (m_pClient->recv(4000) &&
		(m_pSpsJoin->packetType == PACKET_CLIENT_INITIAL_DATA ||
			m_pSpsJoin->packetType == PACKET_CLIENT_JOIN))
	{
		if (applyReceivedGameProperties())
		{
			m_status = clientNS::CLIENT_CONNECTED;
			m_thisClient.setID(m_id);
			return m_pMap->load(m_map);
		}
	}

	disconnect();
	return false;
}

void TanksWar::disconnect()
{
	if (m_status == clientNS::CLIENT_CONNECTED)
	{
		m_pCpsDisconnect->packetType = PACKET_DISCONNECT;
		m_pCpsDisconnect->id = m_id;
		m_pClient->send();
		m_status = clientNS::CLIENT_DISCONNECTED;
	}
	else
		m_status = clientNS::CLIENT_UNCONNECTED;

	m_pClient->disconnect();
	m_pRemoteClient.clear();
	m_pMap->clear();
	m_pInterface->setActivity(interfaceNS::MULTIPLAYER_ACTIVITY);
}

void TanksWar::updateClientInfo()
{
	FileIO::createClientInfo(&m_clientInfo);
}

bool TanksWar::applyReceivedGameProperties()
{
	while (*m_pPacketType == PACKET_CLIENT_INITIAL_DATA)
	{
		auto pRemoteClient = std::make_shared<RemoteClient>(m_pSpsClientInitialData->clientGameState.id, m_pSpsClientInitialData->name, this);
		pRemoteClient->setClientGameState(m_pSpsClientInitialData->clientGameState);
		m_pRemoteClient.push_back(pRemoteClient);
		m_pClient->recv(2000);
	}

	if (*m_pPacketType != PACKET_CLIENT_JOIN)
		return false;

	m_id = m_pSpsJoin->id;
	strcpy(m_map, m_pSpsJoin->map);
	if (!m_pMap->load(m_map))
		return false;

	m_clients = m_pSpsJoin->clients;
	if (m_clients != m_pRemoteClient.size() + 1)
		return false;
	
	return true;
}

void TanksWar::dispatchPlayerAct()
{
	m_pCpsPlayerAct->packetType = PACKET_CLIENT_ACT;
	m_pCpsPlayerAct->id = m_id;
	m_pCpsPlayerAct->act = m_thisClient.getAct();
	m_pClient->send<CpsPlayerAct>();
}

void TanksWar::executeClientAct()
{
	auto clientID = m_pSpsPlayerAct->id;
	auto pClient = findRemoteClientByID(clientID);
	switch (m_pSpsPlayerAct->act)
	{
	case PLAYER_ACT_ATTACK:
		if (pClient)
			pClient->executeAttack();
		else
			m_thisClient.implementAttack();
		break;

	case PLAYER_ACT_DIE:
		if (pClient)
			pClient->executeDie();
		else
			m_thisClient.executeDie();
		break;
	}
}

void TanksWar::heartbeat()
{
	static int64 lastHeartbeatTime = 0;
	int64 currTime = m_pTimer->getCurrentTime();
	if (currTime - lastHeartbeatTime > networkNS::HEARTBEAT_DELAY)
	{
		m_pCpsHeartbeat->packet = PACKET_CLIENT_HEARTBEAT;
		m_pCpsHeartbeat->id = m_id;
		m_pClient->send<CpsHeartbeat>();
		lastHeartbeatTime = currTime;
	}
}

void TanksWar::applyUpdateClientGameState()
{
	auto& updateClientID = m_pSpsClientGameState->clientGameState.id;
	if (m_id == updateClientID)
		m_thisClient.setClientGameState(m_pSpsClientGameState->clientGameState);
	else
		for (auto& pRClient : m_pRemoteClient)
			if (pRClient->getID() == updateClientID)
			{
				pRClient->setClientGameState(m_pSpsClientGameState->clientGameState);
				break;
			}
}

void TanksWar::applyDisconnect()
{
	if (m_pSpsDisconnect->id == m_id)
		disconnect();
	else
	{
		auto pRClient = std::find(m_pRemoteClient.begin(), m_pRemoteClient.end(),
			findRemoteClientByID(m_pSpsDisconnect->id));
		m_pRemoteClient.erase(pRClient);
	}
}

std::shared_ptr<RemoteClient> TanksWar::findRemoteClientByID(PlayerID id)
{
	for (auto& pRClient : m_pRemoteClient)
		if (pRClient->getID() == id)
			return pRClient;

	return nullptr;
}

void TanksWar::setServerPort(Port port)
{
	m_clientInfo.serverPort = port;
	updateClientInfo();
}