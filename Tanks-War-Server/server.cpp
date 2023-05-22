#include "server.h"
#include "..\common\fileio.h"
#include "game.h"

Server::Server() : m_serverPort(netNS::DEFAULT_PORT), m_state(SERVER_NOT_RUNNING), m_pAudio(0), m_pGraphics(0),
m_pTimer(0), m_pTextureManger(0), m_pMap(0)
{
	memset(m_IP, 0, netNS::IP_SIZE);
	memset(m_sIP, 0, netNS::IP_SIZE);
	memset(m_sData, 0, MAX_PACKET_SIZE);
	memset(m_rData, 0, MAX_PACKET_SIZE);
	m_pCpsIni =  (CpsIni*)&m_rData;
	m_pCpsPresent = (CpsPresent*)&m_rData;
	m_pCpsDisconnect = (CpsDisconnect*)&m_rData;
	m_pCpsPlayerAct = (CpsPlayerAct*)&m_rData;
	m_pSpsIni = (SpsIni*)&m_sData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_sData;
	m_pSpsPlayerInitData = (SpsPlayersInitData*)&m_sData;
	m_pSpsDisconnect = (SpsDisconnect*)&m_sData;
	m_pSpsPlayerUpdate = (SpsPlayerUpdate*)&m_sData;
	m_pPacketType = (PacketType*)&m_rData;
}

Server::~Server()
{
}

void Server::initialize(const Game* game)
{
	m_pGame = game;
	m_pMap = game->getMap();
	m_pGraphics = game->getGraphics();
	m_pTextureManger = game->getTextureManger();
	m_pAudio = game->getAudio();
	m_pTimer = game->getTimer();
	ServerInfo info = FileIO::readServerInfo();
	m_serverPort = info.port;
	m_gameMaxPlayers = info.players;
}

void Server::update()
{
	if (!isStarted())
		return;
		for (auto& pClientData : m_pClientData)
			pClientData->update(0);

	//checkClients();
	if (recv())
	{
		switch (*m_pPacketType)
		{
		case PACKET_PLAYERS_EXIST:
			replyPlayersExist();
			break;
		case PACKET_DISCONNECT:
			removeLastClient();
			break;
		case PACKET_PRESENT_CLIENT:
			present();
			break;
		case PACKET_PLAYER_ACT:
			applyPlayerAct();
			break;
		case PACKET_INI:
			if (m_pClientData.size() < m_gameMaxPlayers)
				getClients();
			break;
		}
	}
}

void Server::stop()
{
	m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
	post();
	m_net.closeSocket();
	clearClients();
	m_state = SERVER_NOT_RUNNING;
}

void Server::start()
{
	ServerInfo serverInfo;
	serverInfo.players = m_gameMaxPlayers;
	serverInfo.port = m_serverPort;
	FileIO::createServerInfo(serverInfo);
	if (m_net.createServer(m_serverPort, netNS::UDP) == netNS::NET_OK)
		m_state = SERVER_RUNNING_WAITING;
	
}

void Server::getClients()
{
	if (!addClient())
		return;

	m_pSpsIni->packetType = PACKET_INI;
	m_pSpsIni->gamePlayers = m_gameMaxPlayers;
	strcpy(m_pSpsIni->map, m_pMap->getMap());
	m_pSpsIni->checksum = m_pMap->getCrc32();
	reply();
	postNewPlayer();
	replyPlayersExist();
	replyPlayersIniData();
	postPlayersUpdate();
}

void Server::send(PlayerID id)
{
	int size = MAX_PACKET_SIZE;
	for (auto pClientData : m_pClientData)
	{
		if (pClientData->getID() == id)
		{
			m_net.sendData(m_sData, size, pClientData->getIP(), pClientData->getPort());
			break;
		}
	}

	sbClear();
}

void Server::post()
{
	int size = MAX_PACKET_SIZE;
	for (auto pClient : m_pClientData)
		m_net.sendData(m_sData, size, pClient->getIP(), pClient->getPort());

	sbClear();
}

void Server::postPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI_DATA;
	auto players = m_pClientData.size();
	PlayerIniData* initData = (PlayerIniData*)&m_pSpsPlayerInitData->playerIniData;
	size_t dataSize = sizeof(ClientData)*players;
	memset(initData, 0, dataSize);
	for (int i = 0; i < m_pClientData.size(); i++)
	{
		strcpy(initData[i].name, m_pClientData[i]->getName());
		initData[i].id = m_pClientData[i]->getID();
	}

	post();
}

void Server::postPlayerUpdate(PlayerID id)
{
}

void Server::postPlayersUpdate()
{
	m_pSpsPlayerUpdate->packetType = PACKET_PLAYERS_UPDATE;
	for (int i = 0; i < m_pClientData.size(); i++)
	{
		m_pSpsPlayerUpdate->playerUpdate[i] = m_pClientData[i]->serverPlayer.getPlayerUpdate();
		m_pSpsPlayerUpdate->playerUpdate[i].id = m_pClientData[i]->getID();
	}

	post();
}

void Server::postNewPlayer()
{
	m_pSpsPlayerInitData->packetType = PACKET_NEW_PLAYER;
	m_pSpsPlayerInitData->playerIniData->id = m_pClientData.back()->getID();
	strcpy(m_pSpsPlayerInitData->playerIniData->name, m_pClientData.back()->getName());
	post();
}

void Server::reply()
{
	int size = MAX_PACKET_SIZE;
	m_net.sendData(m_sData, size, m_IP, m_port);
	sbClear();
}

void Server::replyPlayersExist()
{
	m_pSpsPlayersExist->packetType = PACKET_PLAYERS_EXIST;
	m_pSpsPlayersExist->players = m_pClientData.size();
	reply();
}

void Server::replyPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI;
	for (int i = 0; i < m_pClientData.size();i++)
	{
		m_pSpsPlayerInitData->playerIniData[i].id = m_pClientData[i]->getID();
		strcpy(m_pSpsPlayerInitData->playerIniData[i].name, m_pClientData[i]->getName());
	}

	reply();
}

void Server::clearClients()
{
	m_pClientData.clear();
}

PlayerID Server::recvID(bool wait)
{
	bool result = false;
	Port port;
	bool recieved = recv(wait);

	if(recieved)
		for (auto pClientData : m_pClientData)
		{
			if (strcmp(m_IP, pClientData->getIP()) == 0 && m_port == pClientData->getPort())
				return pClientData->getID();
		}

	return result;
}

bool Server::recv(bool wait)
{
	rbClear();
	bool result = false;

	do
	{
		int size = MAX_PACKET_SIZE;
		m_net.readData(m_rData, size, m_IP, m_port);
		if (size > 0)
		{
			result = true;
			break;
		}
	}
	while (wait);

	return result;
}

void Server::present()
{
	if (*m_pPacketType == PACKET_PRESENT_CLIENT)
	{
		for (auto& pClientData : m_pClientData)
			if (pClientData->getID() == getLastRecieverId())
				pClientData->presentTime = m_pTimer->getCurrentTime();
	}
}

void Server::checkClients()
{
	if (m_pClientData.size() > 0)
		for (auto pClientData : m_pClientData)
		{
			auto deltaTime = m_pTimer->getCurrentTime() - pClientData->presentTime;
			if (deltaTime > SERVER_RECIEVE_PRESENT_TIME)
			{
				PlayerID id = pClientData->getID();
				m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
				send(id);
				removeClient(id);
				break;
			}
		}
}

ClientData* Server::getIDClientData(PlayerID requiredID)
{
	for (auto &cd : m_pClientData)
		if (requiredID == cd->getID())
			return cd.get();
}

void Server::applyPlayerAct()
{
	PlayerID& id = m_pCpsPlayerAct->id;
	ServerPlayer& serverPlayer = getIDClientData(id)->serverPlayer;
	
	switch (m_pCpsPlayerAct->act)
	{
	case PLAYER_ACT_FORWRAD:
		serverPlayer.executeForward(1);
		break;
	case PLAYER_ACT_BACK:
		serverPlayer.executeBack(1);
		break;
	case PLAYER_ACT_RIGHT:
		serverPlayer.executeRight(1);
		break;
	case PLAYER_ACT_LEFT:
		serverPlayer.executeLeft(1);
		break;
	case PLAYER_ACT_ATTACK:
		serverPlayer.executeAttack();
		break;
	case PLAYER_ACT_FORWARD_LEFT:
		serverPlayer.executeLeft(1);
		serverPlayer.executeForward(1);
		break;
	case PLAYER_ACT_FORWARD_RIGHT:
		serverPlayer.executeRight(1);
		serverPlayer.executeForward(1);
		break;
	case PLAYER_ACT_BACK_LEFT:
		serverPlayer.executeLeft(1);
		serverPlayer.executeBack(1);
		break;
	case PLAYER_ACT_BACK_RIGHT:
		serverPlayer.executeRight(1);
		serverPlayer.executeBack(1);
		break;
	default:
		throw GameError(gameErrorNS::WARNING, "Unknown player act has been recieved!\n The other player(s) may have a different game version.");
	}

	m_pSpsPlayerUpdate->packetType = PACKET_PLAYER_UPDATE;
	m_pSpsPlayerUpdate->playerUpdate[0] = serverPlayer.getPlayerUpdate();
	m_pSpsPlayerUpdate->playerUpdate[0].id = id;
	post();
}

bool Server::addClient()
{
	for (auto pClientData : m_pClientData)
		if (pClientData->getPort() == m_port)
			if(strcmp(pClientData->getIP(), m_IP) == 0)
			return false;
	
	PlayerID id = generateID();
	m_pSpsIni->id = id;
	std::shared_ptr<ClientData> pClientData = std::make_shared<ClientData>(id, m_pCpsIni->name, m_IP, m_port,m_pGame);
	Space space = m_pMap->getRandomEmptySpace();
	V3 position = V3(space.v1.x, space.v1.y, 0);
	pClientData->serverPlayer.setPosition(position);
	m_pClientData.push_back(pClientData);
	return true;
}

void Server::removeLastClient()
{
	removeClient(m_pCpsDisconnect->id);
	return;
}

void Server::removeClient(PlayerID id)
{
	for (int i = 0; i < m_pClientData.size(); i++)
		if (m_pClientData[i]->getID() == id)
		{
			m_pClientData.erase(std::next(m_pClientData.begin(), i));
			m_pSpsDisconnect->packetType = PACKET_PLAYER_DISCONNECTED;
			m_pSpsDisconnect->id = id;
			post();
		}
}

PlayerID Server::getLastRecieverId()
{
	PlayerID id = INVALID_ID;
	for (auto pClientData : m_pClientData)
	{
		if (strcmp(pClientData->getIP() , m_IP) == 0 && pClientData->getPort() == m_port)
		{
			id = pClientData->getID();
			break;
		}
	}

	return id;
}
