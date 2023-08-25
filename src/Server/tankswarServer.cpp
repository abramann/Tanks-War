// tankswarserver.cpp
// Author: abramann

#include "tankswarserver.h"
#include "..\common\fileio.h"
#include "..\common\texturemanger.h"
#include "..\common\interface.h"
#include "..\common\map.h"
#include "..\common\serverplayer.h"
#include "..\common\timer.h"

namespace tanksWarServerNS
{
	auto CLIENT_TIMEOUT = 60000;
}

TanksWarServer::TanksWarServer() : m_active(false), m_status(SERVER_NOT_RUNNING)
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

	m_pSpsDisconnect = (SpsDisconnect*)m_pSData;
	m_pSpsJoin = (SpsJoin*)m_pSData;
	m_pSpsClientGameState = (SpsClientGameState*)m_pSData;
}

TanksWarServer::~TanksWarServer()
{
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
	if (m_active)
	{
		communicate();
		updateGameState();
	}
}

void TanksWarServer::updateGameState()
{
}

void TanksWarServer::render()
{
	m_pInterface->render();
}

void TanksWarServer::communicate()
{
	if (!recv)
		return;

	if (m_pClient.size() < m_maxClients)
		handleNewClient();

	receiveClientHeartbeat();
	disonnectInactiveClient();
}

void TanksWarServer::handleNewClient()
{
	if (*m_pPacketType == PACKET_CLIENT_JOIN)
		if (clientExist())
			return;

	createClient();
}

void TanksWarServer::createClient()
{
	PlayerID uniqueID = generateClientID();
	auto pClient = make_shared<Client>();// (uniqueID, m_pCpsJoin->name, m_pReceiverIP, *m_pReceiverPort, this);
	m_pSpsJoin->id = uniqueID;
	strcpy(m_pSpsJoin->name, m_pSpsJoin->name);
	post(sizeof(m_pSpsJoin));
	resetClientGameState(pClient.get());
}

bool TanksWarServer::clientExist()
{
	for (auto pClient : m_pClient)
		if (strcmp(m_pReceiverIP, pClient->getIP()) == 0 && *m_pReceiverPort == pClient->getPort())
			return true;

	return false;
}

void TanksWarServer::receiveClientHeartbeat()
{
	if (*m_pPacketType == PACKET_CLIENT_HEARTBEAT)
	{
		int64 currTime = m_pTimer->getCurrentTime();
		m_pReceiverClient->setHeartbeatTime(currTime);
	}
}

void TanksWarServer::disconnectClient(std::shared_ptr<Client> pClient)
{
	auto ite = std::find(m_pClient.begin(), m_pClient.end(), pClient);
	m_pSpsDisconnect->packetType = PACKET_CLIENT_DISCONNECTED;
	m_pSpsDisconnect->id = pClient->getID();
	m_pClient.erase(ite);
	post(sizeof(SpsDisconnect));
}

void TanksWarServer::disonnectInactiveClient()
{
	int64 currTime = m_pTimer->getCurrentTime();
	for (auto pClient : m_pClient)
	{
		int64 delta = currTime - pClient->getHeartbeatTime();
		if (delta > tanksWarServerNS::CLIENT_TIMEOUT)
			disconnectClient(pClient);
	}
}

void TanksWarServer::serverStart()
{
	if (m_pServer->start())
	{
		m_active = true;
		m_status = SERVER_RUNNING_HANDLING;
		if (m_map.empty())
			m_map = getRandomMap();

		if (!m_pMap->load(m_map.c_str()))
			serverShutdown();
	}

}

void TanksWarServer::serverShutdown()
{
	m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
	post();
	m_pServer->close();
	m_pClient.clear();
	m_pMap->clear();
	m_active = false;
	m_status = SERVER_DISCONNECTED;
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

void TanksWarServer::post(int size)
{
	for (auto pClient : m_pClient)
		send(pClient.get(), size);
}

void TanksWarServer::send(Client* pClient, int size)
{
	m_pServer->send(pClient->getIP(), pClient->getPort(), size);
}

void TanksWarServer::resetClientGameState(Client* pClient)
{
	Space space = m_pMap->getRandomEmptySpace();
	V3 position = V3(space.v1.x, space.v1.y, 0);
	pClient->setPosition(position);
	updateClientGameState(pClient);
}

void TanksWarServer::updateClientGameState(Client* pClient)
{
	m_pSpsClientGameState->packetType = PACKET_CLIENT_GAME_STATE;
	m_pSpsClientGameState->id = pClient->getID();
	m_pSpsClientGameState->position = pClient->getPosition();
	m_pSpsClientGameState->rotate = pClient->getRotate();
	m_pSpsClientGameState->health = pClient->getHealth();
	m_pSpsClientGameState->velocity = pClient->getVelocity();
	post(sizeof(SpsClientGameState));
}

void TanksWarServer::setPort(Port port)
{
	m_port = port;
	ServerInfo info;
	info.port = port;
	info.maxClients = m_maxClients;
	FileIO::createServerInfo(info);
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
			if (pClient->getPort() == *m_pReceiverPort && strcmp(pClient->getIP, m_pReceiverIP) == 0)
			{
				m_pReceiverClient = pClient;
				return true;
			}
	}

	return false;
}
