// tankswarserver.cpp
// Author: abramann

#include "tankswarserver.h"
#include "..\common\fileio.h"
#include "..\common\texturemanger.h"
#include "..\common\interface.h"
#include "..\common\map.h"
#include "..\common\serverplayer.h"
#include "..\common\timer.h"
#include "..\common\inlined.inl"

using namespace serverNS;

TanksWarServer::TanksWarServer() : m_status(SERVER_NOT_RUNNING)
{
	m_pServer = std::make_shared<Server>();

	m_pRData = m_pServer->getReceiveBuffer();
	m_pSData = m_pServer->getSendBuffer();
	m_pReceiverIP = m_pServer->getReceiverIP();
	m_pReceiverPort = m_pServer->getReceiverPort();
	m_pPacketType = (PacketType*)m_pRData;

	m_pCpsJoin = (CpsJoin*)m_pRData;
	m_pCpsHeartbeat = (CpsHeartbeat*)m_pRData;
	m_pCpsDisconnect = (CpsDisconnect*)m_pRData;
	m_pCpsPlayerAct = (CpsPlayerAct*)m_pRData;

	m_pSpsDisconnect = (SpsDisconnect*)m_pSData;
	m_pSpsJoin = (SpsJoin*)m_pSData;
	m_pSpsClientInitialData = (SpsClientInitialData*)m_pSData;
	m_pSpsClientGameStatus = (SpsClientGameStatus*)m_pSData;
	m_pSpsPlayerAct = (SpsPlayerAct*)m_pSData;
	m_pSpsClientGameAttribute = (SpsClientGameAttribute*)m_pSData;
}

TanksWarServer::~TanksWarServer()
{
	serverShutdown();
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
	m_pInterface->initialize(this);
	m_pServer->initialize(this);
	ServerInfo info = FileIO::readServerInfo();
	m_port = info.port;
	m_maxClients = info.maxClients;
}

void TanksWarServer::update()
{
	updateScene();
}

void TanksWarServer::updateScene()
{
	for (auto& pClient : m_pClient)
		pClient->update();
}

void TanksWarServer::render()
{
	m_pInterface->render();
}

void TanksWarServer::communicate()
{
#ifndef _DEBUG
	disonnectInactiveClient();
#endif
	if (!recv())
		return;

	switch (*m_pPacketType)
	{
	case PACKET_CLIENT_JOIN:
		handleNewClient();
		break;

	case PACKET_DISCONNECT:
		disconnectClient(m_ppReceiverClient);
		break;

	case PACKET_CLIENT_HEARTBEAT:
		applyClientHeartbeat();
		break;

	case PACKET_CLIENT_ACT:
		applyClientPlayerAct();
		break;

	default:
		break;
	}

}

void TanksWarServer::handleNewClient()
{
	if ((m_pClient.size() == m_maxClients) || clientExist())
		return;

		createClient();
}

void TanksWarServer::createClient()
{
	PlayerID uniqueID = generateClientID();
	replyClientsInitialData();
	m_pClient.push_back(make_unique<Client>(uniqueID, m_pCpsJoin->name, m_pReceiverIP, *m_pReceiverPort, this));
	m_pSpsJoin->packetType = PACKET_CLIENT_JOIN;
	m_pSpsJoin->id = uniqueID;
	m_pSpsJoin->clients = m_pClient.size();
	strcpy(m_pSpsJoin->name, m_pCpsJoin->name);
	strcpy(m_pSpsJoin->map, m_map.c_str());
	resetClientGameStatus(m_pClient.back().get());
	post<SpsJoin>();
	postClientGameStatus(m_pClient.back().get());
	m_pClient.back()->setHeartbeatTime(m_pTimer->getCurrentTime());
	postClientGameAttribute(nullptr);
}

bool TanksWarServer::clientExist()
{
	for (auto& pClient : m_pClient)
		if (strcmp(m_pReceiverIP, pClient->getIP()) == 0 && *m_pReceiverPort == pClient->getPort())
			return true;

	return false;
}

void TanksWarServer::applyClientHeartbeat()
{
	int64 currTime = m_pTimer->getCurrentTime();
	m_ppReceiverClient->get()->setHeartbeatTime(currTime);
}

void TanksWarServer::applyClientPlayerAct()
{
	switch (m_pCpsPlayerAct->act)
	{
	case PLAYER_ACT_FORWRAD:
		m_ppReceiverClient->get()->executeForward();
		break;

	case PLAYER_ACT_BACK:
		m_ppReceiverClient->get()->executeBack();
		break;
	case PLAYER_ACT_RIGHT:
		m_ppReceiverClient->get()->executeRight();
		break;

	case PLAYER_ACT_LEFT:
		m_ppReceiverClient->get()->executeLeft();
		break;

	case PLAYER_ACT_FORWARD_RIGHT:
		m_ppReceiverClient->get()->executeForward();
		m_ppReceiverClient->get()->executeRight();
		break;

	case PLAYER_ACT_FORWARD_LEFT:
		m_ppReceiverClient->get()->executeForward();
		m_ppReceiverClient->get()->executeLeft();
		break;

	case PLAYER_ACT_BACK_RIGHT:
		m_ppReceiverClient->get()->executeBack();
		m_ppReceiverClient->get()->executeRight();
		break;

	case PLAYER_ACT_BACK_LEFT:
		m_ppReceiverClient->get()->executeBack();
		m_ppReceiverClient->get()->executeLeft();
		break;

	case PLAYER_ACT_ATTACK:
		if (m_ppReceiverClient->get()->executeAttack())
		{
			m_pSpsPlayerAct->packetType = PACKET_CLIENT_ACT;
			m_pSpsPlayerAct->id = m_ppReceiverClient->get()->getID();
			m_pSpsPlayerAct->act = PLAYER_ACT_ATTACK;
			post<SpsPlayerAct>();
		}

		break;

	default:
		break;
	}

	m_pSpsClientGameStatus->packetType = PACKET_CLIENT_GAME_STATE;
	m_pSpsClientGameStatus->clientGameStatus = m_ppReceiverClient->get()->getClientGameStatus();
	post<SpsClientGameStatus>();
}

void TanksWarServer::disconnectClient(std::unique_ptr<Client>* ppClient)
{
	if (m_pClient.size() == 0)
		throw GameError(0, "Error disconnecting client with empty vector");

	auto disconnectedClient = std::find(m_pClient.begin(), m_pClient.end(), *ppClient);
	m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
	m_pSpsDisconnect->id = ppClient->get()->getID();
	post<SpsDisconnect>();
	m_pClient.erase(disconnectedClient);
}

void TanksWarServer::disonnectInactiveClient()
{
	int64 currTime = m_pTimer->getCurrentTime();
	for (size_t i = 0; i < m_pClient.size(); i++)
	{
		int64 delta = currTime - m_pClient[i]->getHeartbeatTime();
		if (delta > networkNS::SERVER_TIME_OUT)
			disconnectClient(&m_pClient[i]);
	}
}

void TanksWarServer::serverStart()
{
	if (m_pServer->start())
	{
		m_status = SERVER_RUNNING_HANDLING;
		if (m_map.empty())
			m_map = getRandomMap();

		if (!m_pMap->load(m_map.c_str()))
			serverShutdown();
	}

}

void TanksWarServer::serverShutdown()
{
	if (m_status == serverNS::SERVER_RUNNING_HANDLING)
	{
		m_pSpsDisconnect->packetType = PACKET_SERVER_SHUTDOWN;
		post();
		m_status = serverNS::SERVER_NOT_RUNNING;
	}
	else
		m_status = SERVER_DISCONNECTED;

	m_pServer->close();
	m_pClient.clear();
	m_pMap->clear();
}

void TanksWarServer::applyPlayerMove()
{
	/*PlayerID& id = m_pCpsPlayerAct->id;
	ServerPlayer& serverPlayer = getIDClientData(id)->serverPlayer;
	PlayerAct& act = m_pCpsPlayerAct->act;
	if (act >= PLAYER_ACT_ATTACK)
	{
		act -= PLAYER_ACT_ATTACK;
		if (!serverPlayer.isBulletLaunching())
		{
			serverPlayer.executeAttack();
			m_pSpsPlayerAct->packetType = PACKET_CLIENT_ACT;
			m_pSpsPlayerAct->id = id;
			m_pSpsPlayerAct->act = PLAYER_ACT_ATTACK;
			post(sizeof(m_pSpsPlayerAct));
		}
	}

	switch (act)
	{
	case PLAYER_ACT_NONE:
		break;
	case PLAYER_ACT_FORWRAD:
		serverPlayer.executeForward();
		break;
	case PLAYER_ACT_BACK:
		serverPlayer.executeBack();
		break;
	case PLAYER_ACT_RIGHT:
		serverPlayer.executeRight();
		break;
	case PLAYER_ACT_LEFT:
		serverPlayer.executeLeft();
		break;
	case PLAYER_ACT_ATTACK:
		serverPlayer.executeAttack();
		break;
	case PLAYER_ACT_FORWARD_LEFT:
		serverPlayer.executeLeft();
		serverPlayer.executeForward();
		break;
	case PLAYER_ACT_FORWARD_RIGHT:
		serverPlayer.executeRight();
		serverPlayer.executeForward();
		break;
	case PLAYER_ACT_BACK_LEFT:
		serverPlayer.executeLeft();
		serverPlayer.executeBack();
		break;
	case PLAYER_ACT_BACK_RIGHT:
		serverPlayer.executeRight();
		serverPlayer.executeBack();
		break;
	default:
		throw GameError(gameErrorNS::WARNING, "Unknown player act has been recieved!\n The other player(s) may have a different game version.");
	}

	postPlayerUpdate(id);*/
}

void TanksWarServer::post(int32 size)
{
	for (auto& pClient : m_pClient)
		send(pClient.get(), size, false);
	
	m_pServer->sbClear();
}

void TanksWarServer::send(Client* pClient, int32 size, bool sbClear)
{
	m_pServer->send(pClient->getIP(), pClient->getPort(), size, sbClear);
}

void TanksWarServer::resetClientGameStatus(Client* pClient)
{
	Space space = m_pMap->getRandomEmptySpace();
	V3 position = V3(space.v1.x, space.v1.y, 0);
	pClient->setPosition(position);
	pClient->setRotate(V3(0, 0, 0));
	pClient->setHealth(logicNS::HEALTH_TANK);
}

void TanksWarServer::reply(int32 size)
{
	m_pServer->send(m_pReceiverIP, *m_pReceiverPort, size);
}

void TanksWarServer::postClientGameStatus(Client* pClient)
{
	m_pSpsClientGameStatus->packetType = PACKET_CLIENT_GAME_STATE;
	m_pSpsClientGameStatus->clientGameStatus = pClient->getClientGameStatus();
	post<SpsClientGameStatus>();
}

void TanksWarServer::setPort(Port port)
{
	m_port = port;
	ServerInfo info;
	info.port = port;
	info.maxClients = m_maxClients;
	FileIO::createServerInfo(info);
}

void TanksWarServer::replyClientsInitialData()
{
	for (auto& pClient : m_pClient)
	{
		m_pSpsClientInitialData->packetType = PACKET_CLIENT_INITIAL_DATA;
		strcpy(m_pSpsClientInitialData->name, pClient->getName());
		m_pSpsClientInitialData->clientGameStatus = pClient->getClientGameStatus();
		reply<SpsClientInitialData>();
	}
}

void TanksWarServer::setMaxClients(int32 maxClients)
{
	m_maxClients = maxClients;
	ServerInfo info;
	info.maxClients = m_maxClients;
	info.port = m_port;
	FileIO::createServerInfo(info);
}

PlayerID TanksWarServer::generateClientID() const
{
	static std::vector<PlayerID> clientID;
	PlayerID id = 0;
	do
		id = random(1, 255);
	while (std::binary_search(clientID.begin(),clientID.end(), id) || id == 0);

	clientID.push_back(id);
	return id;
}

std::string TanksWarServer::getRandomMap() const
{
	std::vector<std::string> mapList = FileIO::getDirFileList(fileNS::MAP_DIR);
	std::string map = mapList[random(0, mapList.size() - 1)];
	map = map.substr(0, map.size() - 4);
	return map;
}

bool TanksWarServer::recv()
{
	if (m_pServer->recv())
	{
		for (auto& pClient : m_pClient)
			if (pClient->getPort() == *m_pReceiverPort && strcmp(pClient->getIP(), m_pReceiverIP) == 0)
				m_ppReceiverClient = &pClient;

		return true;
	}

	return false;
}

void TanksWarServer::executeServerPlayerDie(Client* pClient)
{
	m_pSpsPlayerAct->id = pClient->getID();
	m_pSpsPlayerAct->packetType = PACKET_CLIENT_ACT;
	m_pSpsPlayerAct->act = PLAYER_ACT_DIE;
	post<SpsPlayerAct>();
}

void TanksWarServer::postClientGameAttribute(Client * pClient)
{
	if (pClient)
	{
		m_pSpsClientGameAttribute->packetType = PACKET_CLIENT_GAME_ATTRIBUTE;
		m_pSpsClientGameAttribute->clientGameAttribute[0] = pClient->getClientGameAttribute();
		m_pSpsClientGameAttribute->clients = 1;
		post(sizeof(SpsClientGameAttribute) + sizeof(ClientGameAttribute));
	}
	else	// update all
	{
		m_pSpsClientGameAttribute->packetType = PACKET_CLIENT_GAME_ATTRIBUTE;
		m_pSpsClientGameAttribute->clients = m_pClient.size();
		for (size_t i = 0; i < m_pClient.size(); i++)
			m_pSpsClientGameAttribute->clientGameAttribute[i] = m_pClient[i]->getClientGameAttribute();
	
		post(sizeof(SpsClientGameAttribute) + sizeof(ClientGameAttribute) * m_pClient.size());

	}
}
