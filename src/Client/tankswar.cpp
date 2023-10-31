// tankswar.cpp
// Author: abramann

#include "TanksWar.h"
#include "..\common\interface.h"
#include "..\common\input.h"
#include "..\common\fileio.h"
#include "..\common\data.h"
#include "..\common\serverplayer.h"
#include "..\common\timer.h"
#include "..\common\aiplayer.h"
#include "..\common\inlined.inl"
#include "thisplayer.h"

TanksWar::TanksWar() : m_status(clientNS::CLIENT_UNCONNECTED), m_aiLevel(AI_LEVEL_EASY), m_soloGameStarted(false), m_AIPlayersCount(0)
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
	m_pSpsClientGameStatus = (SpsClientGameStatus*)m_pRData;
	m_pSpsDisconnect = (SpsDisconnect*)m_pRData;
	m_pSpsPlayerAct = (SpsPlayerAct*)m_pRData;
	m_pSpsClientGameAttribute = (SpsClientGameAttribute*)m_pRData;
}

TanksWar::~TanksWar()
{
	if (m_status == clientNS::CLIENT_CONNECTED)
		disconnect();
}

void TanksWar::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
	m_pClient->initialize(this);
	m_pInterface->initialize(this);
	m_thisClient.initialize(0, this);
	m_thisPlayer.initialize(this);
	m_clientInfo = FileIO::readClientInfo();
}

void TanksWar::update()
{
}

void TanksWar::render()
{
	m_pInterface->render();
}

void TanksWar::renderScene()
{
	m_pMap->draw();
	if (isOnline())
	{
		m_thisClient.draw();
		for (auto& pRClient : m_pRemoteClient)
			pRClient->draw();
	}
	else
	{
		m_thisPlayer.draw();
		for (auto& pAIPlayer : m_pAIPlayer)
			pAIPlayer->draw();
	}
}

void TanksWar::updateScene()
{
	if (isOnline())
	{
		m_thisClient.update();
		for (auto& pRClient : m_pRemoteClient)
			pRClient->update();
	}
	else
	{
		m_thisPlayer.update();
		for (auto& pAIPlayer : m_pAIPlayer)
			pAIPlayer->update();
	}
}

void TanksWar::communicate()
{
#ifndef _DEBUG
	heartbeat();
#endif
	if (!m_pClient->recv())
		return;

	switch (*m_pPacketType)
	{
	case PACKET_CLIENT_GAME_STATE:
		applyUpdateClientGameStatus();
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
		break;

	case PACKET_CLIENT_GAME_ATTRIBUTE:
		applyClientGameAttribute();
		break;

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
		auto pRemoteClient = std::make_shared<RemoteClient>(m_pSpsClientInitialData->clientGameStatus.id, m_pSpsClientInitialData->name, this);
		pRemoteClient->setClientGameStatus(m_pSpsClientInitialData->clientGameStatus);
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

bool TanksWar::onStartSoloPlayerGame()
{
	if (!m_pMap->load(m_map))
		return false;

	m_thisPlayer.setPosition(m_pMap->getRandomEmptySpace(&m_thisPlayer).v1);
	m_pAIPlayer.resize(m_AIPlayersCount);
	uint8 c = 1;
	for (auto& pAIPlayer : m_pAIPlayer)
	{
		pAIPlayer->initialize(this, c, strFormat("AIPlayer %d", c++).c_str());
		pAIPlayer->setPosition(m_pMap->getRandomEmptySpace(pAIPlayer.get()).v1);
	}
	m_soloGameStarted = true;
	return true;
}

void TanksWar::quitSoloGame()
{
	m_pAIPlayer.clear();
	m_pMap->unload();
	m_soloGameStarted = false;
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

void TanksWar::applyUpdateClientGameStatus()
{
	auto& updateClientID = m_pSpsClientGameStatus->clientGameStatus.id;
	if (m_id == updateClientID)
		m_thisClient.setClientGameStatus(m_pSpsClientGameStatus->clientGameStatus);
	else
		for (auto& pRClient : m_pRemoteClient)
			if (pRClient->getID() == updateClientID)
			{
				pRClient->setClientGameStatus(m_pSpsClientGameStatus->clientGameStatus);
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

void TanksWar::applyClientGameAttribute()
{
	for (int8 i = 0; i < m_pSpsClientGameAttribute->clients; i++)
	{
		ClientGameAttribute& cga = m_pSpsClientGameAttribute->clientGameAttribute[i];
		if (cga.id == m_id)
		{
			m_thisClient.setClientGameAttribute(cga);
		}
		else
		{
			auto remoteClient = findRemoteClientByID(cga.id);
			debuggerBreak();
			remoteClient->setClientGameAttribute(cga);
		}
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