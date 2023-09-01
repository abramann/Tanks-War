// tankswarserver.cpp
// Author: abramann

#include "tankswarserver.h"
#include "..\common\fileio.h"
#include "..\common\texturemanger.h"
#include "..\common\interface.h"
#include "..\common\map.h"
#include "..\common\serverplayer.h"
#include "..\common\timer.h"

using namespace serverNS;
namespace tanksWarServerNS
{
	auto CLIENT_TIMEOUT = 60000;
}

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
	m_pSpsClientGameState = (SpsClientGameState*)m_pSData;
	m_pSpsPlayerAct = (SpsPlayerAct*)m_pSData;
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
	for (auto pClient : m_pClient)
		pClient->update();
}

void TanksWarServer::render()
{
	m_pInterface->render();
}

void TanksWarServer::communicate()
{
	//disonnectInactiveClient();
	if (!recv())
		return;

	switch (*m_pPacketType)
	{
	case PACKET_CLIENT_JOIN:
		handleNewClient();
		break;

	case PACKET_DISCONNECT:
		disconnectClient(m_pReceiverClient);
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
	auto pClient = make_shared<Client>(uniqueID, m_pCpsJoin->name, m_pReceiverIP, *m_pReceiverPort, this);
	replyClientsInitialData();
	m_pClient.push_back(pClient);
	m_pSpsJoin->packetType = PACKET_CLIENT_JOIN;
	m_pSpsJoin->id = uniqueID;
	m_pSpsJoin->clients = m_pClient.size();
	strcpy(m_pSpsJoin->name, m_pCpsJoin->name);
	strcpy(m_pSpsJoin->map, m_map.c_str());
	resetClientGameState(pClient.get());
	post<SpsJoin>();
	postClientGameState(pClient.get());
}

bool TanksWarServer::clientExist()
{
	for (auto pClient : m_pClient)
		if (strcmp(m_pReceiverIP, pClient->getIP()) == 0 && *m_pReceiverPort == pClient->getPort())
			return true;

	return false;
}

void TanksWarServer::applyClientHeartbeat()
{
	int64 currTime = m_pTimer->getCurrentTime();
	m_pReceiverClient->setHeartbeatTime(currTime);
}

void TanksWarServer::applyClientPlayerAct()
{
	switch (m_pCpsPlayerAct->act)
	{
	case PLAYER_ACT_FORWRAD:
		m_pReceiverClient->executeForward();
		break;

	case PLAYER_ACT_BACK:
		m_pReceiverClient->executeBack();
		break;
	case PLAYER_ACT_RIGHT:
		m_pReceiverClient->executeRight();
		break;

	case PLAYER_ACT_LEFT:
		m_pReceiverClient->executeLeft();
		break;

	case PLAYER_ACT_FORWARD_RIGHT:
		m_pReceiverClient->executeForward();
		m_pReceiverClient->executeRight();
		break;

	case PLAYER_ACT_FORWARD_LEFT:
		m_pReceiverClient->executeForward();
		m_pReceiverClient->executeLeft();
		break;

	case PLAYER_ACT_BACK_RIGHT:
		m_pReceiverClient->executeBack();
		m_pReceiverClient->executeRight();
		break;

	case PLAYER_ACT_BACK_LEFT:
		m_pReceiverClient->executeBack();
		m_pReceiverClient->executeLeft();
		break;

	case PLAYER_ACT_ATTACK:
		if (m_pReceiverClient->executeAttack())
		{
			m_pSpsPlayerAct->packetType = PACKET_CLIENT_ACT;
			m_pSpsPlayerAct->id = m_pReceiverClient->getID();
			m_pSpsPlayerAct->act = PLAYER_ACT_ATTACK;
			post<SpsPlayerAct>();
		}

		break;

	default:
		break;
	}

	m_pSpsClientGameState->packetType = PACKET_CLIENT_GAME_STATE;
	m_pSpsClientGameState->clientGameState = m_pReceiverClient->getClientGameState();
	post<SpsClientGameState>();
}

void TanksWarServer::disconnectClient(std::shared_ptr<Client> pClient)
{
	if (m_pClient.size() == 0)
		throw GameError(0, "Error disconnicting client with empty vector");

	auto disconnectedClient = std::find(m_pClient.begin(), m_pClient.end(), pClient);
	m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
	m_pSpsDisconnect->id = pClient->getID();
	post<SpsDisconnect>();
	m_pClient.erase(disconnectedClient);
}

void TanksWarServer::disonnectInactiveClient()
{
	int64 currTime = m_pTimer->getCurrentTime();
	for (size_t i = 0; i < m_pClient.size(); i++)
	{
		int64 delta = currTime - m_pClient[i]->getHeartbeatTime();
		if (delta > networkNS::HEARTBEAT_DELAY)
			disconnectClient(m_pClient[i]);
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
	for (auto pClient : m_pClient)
		send(pClient.get(), size, false);
	
	m_pServer->sbClear();
}

void TanksWarServer::send(Client* pClient, int32 size, bool sbClear)
{
	m_pServer->send(pClient->getIP(), pClient->getPort(), size, sbClear);
}

void TanksWarServer::resetClientGameState(Client* pClient)
{
	Space space = m_pMap->getRandomEmptySpace();
	V3 position = V3(space.v1.x, space.v1.y, 0);
	pClient->setPosition(position);
	pClient->setHeartbeatTime(m_pTimer->getCurrentTime());
}

void TanksWarServer::reply(int32 size)
{
	m_pServer->send(m_pReceiverIP, *m_pReceiverPort, size);
}

void TanksWarServer::postClientGameState(Client* pClient)
{
	m_pSpsClientGameState->packetType = PACKET_CLIENT_GAME_STATE;
	m_pSpsClientGameState->clientGameState = pClient->getClientGameState();
	post<SpsClientGameState>();
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
	for (auto pClient : m_pClient)
	{
		m_pSpsClientInitialData->packetType = PACKET_CLIENT_INITIAL_DATA;
		strcpy(m_pSpsClientInitialData->name, pClient->getName());
		m_pSpsClientInitialData->clientGameState = pClient->getClientGameState();
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
		id = _rand(255);
	while (std::binary_search(clientID.begin(),clientID.end(), id) || id == 0);

	clientID.push_back(id);
	return id;
}

std::string TanksWarServer::getRandomMap() const
{
	std::vector<std::string> mapList = FileIO::getDirFileList(fileNS::MAP_DIR);
	std::string map = mapList[_rand(mapList.size() - 1)];
	map = map.substr(0, map.size() - 4);
	return map;
}

bool TanksWarServer::recv()
{
	if (m_pServer->recv())
	{
		for (auto pClient : m_pClient)
			if (pClient->getPort() == *m_pReceiverPort && strcmp(pClient->getIP(), m_pReceiverIP) == 0)
				m_pReceiverClient = pClient;

		return true;
	}

	return false;
}

void TanksWarServer::executeClientPlayerDie(Client* pClient)
{
	m_pSpsPlayerAct->id = pClient->getID();
	m_pSpsPlayerAct->packetType = PACKET_CLIENT_ACT;
	m_pSpsPlayerAct->act = PLAYER_ACT_DIE;
	post<SpsPlayerAct>();
}
