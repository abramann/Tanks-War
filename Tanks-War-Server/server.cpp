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
//	strcpy(m_map, info.map);
}

void Server::update()
{
	if (!isStarted())
		return;
		for (auto& clientData : m_clientData)
			clientData.serverPlayer.update(0);

	

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
			if (m_clientData.size() < m_gameMaxPlayers)
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
	if (addClient())
		m_pSpsIni->accept = true;
	else
		m_pSpsIni->accept = false;

	m_pSpsIni->packetType = PACKET_INI;

	m_pSpsIni->gamePlayers = m_gameMaxPlayers;
	strcpy(m_pSpsIni->map, m_pMap->getMap());
	m_pSpsIni->checksum = m_pMap->getCrc32();
	reply();
	/*	recv();
 if (*m_pPacketType != PACKET_START_SEASSON)
	{
		m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
		reply();
		removeLastClient();
		return;
	}*/

	postNewPlayer();
	replyPlayersExist();
	replayPlayersIniData();
	postPlayersUpdate();
}

void Server::send(PlayerID id)
{
	int size = MAX_PACKET_SIZE;
	for (auto clientData : m_clientData)
	{
		if (clientData.getID() == id)
		{
			m_net.sendData(m_sData, size, clientData.ip, clientData.port);
			break;
		}
	}

	sbClear();
}

void Server::post()
{
	int size = MAX_PACKET_SIZE;
	for (int i = 0; i < m_clientData.size(); i++)
		m_net.sendData(m_sData, size, m_clientData[i].ip, m_clientData[i].port);

	sbClear();
}

void Server::postPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI_DATA;
	auto players = m_clientData.size();
	PlayerIniData* initData = (PlayerIniData*)&m_pSpsPlayerInitData->playerIniData;
	size_t dataSize = sizeof(ClientData)*players;
	memset(initData, 0, dataSize);
	uint8_t i = 0;
	for (int i = 0; i < m_clientData.size(); i++)
	{
		ClientData& clientData = m_clientData[i];
		strcpy(initData[i].name, clientData.name);
		initData[i].id = clientData.getID();
	}

	post();
}

void Server::postPlayerUpdate(PlayerID id)
{
}

void Server::postPlayersUpdate()
{
	m_pSpsPlayerUpdate->packetType = PACKET_PLAYERS_UPDATE;
	for (int i = 0; i < m_clientData.size(); i++)
	{
		m_pSpsPlayerUpdate->playerUpdate[i] = m_clientData[i].serverPlayer.getPlayerUpdate();
		m_pSpsPlayerUpdate->playerUpdate[i].id = m_clientData[i].getID();
	}

	post();
}

void Server::postNewPlayer()
{
	m_pSpsPlayerInitData->packetType = PACKET_NEW_PLAYER;
	m_pSpsPlayerInitData->playerIniData->id = m_clientData.back().getID();
	strcpy(m_pSpsPlayerInitData->playerIniData->name, m_clientData.back().name);
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
	m_pSpsPlayersExist->players = m_clientData.size();
	reply();
}

void Server::replayPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI;
	for (int i = 0; i < m_clientData.size();i++)
	{
		m_pSpsPlayerInitData->playerIniData[i].id = m_clientData[i].getID();
		strcpy(m_pSpsPlayerInitData->playerIniData[i].name, m_clientData[i].getName());
	}

	reply();
}

void Server::clearClients()
{
	m_clientData.clear();
}

PlayerID Server::recvID(bool wait)
{
	bool result = false;
	Port port;
	bool recieved = recv(wait);

	if(recieved)
		for (auto client : m_clientData)
		{
			if (strcmp(m_IP, client.ip) == 0 && m_port == client.port)
				return client.getID();
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
		for (auto& clientData : m_clientData)
			if (clientData.getID() == getLastRecieverId())
				clientData.presentTime = m_pTimer->getCurrentTime();
	}
}

void Server::checkClients()
{
	if (m_clientData.size() > 0)
		for (auto clientData : m_clientData)
		{
			auto deltaTime = m_pTimer->getCurrentTime() - clientData.presentTime;
			if (deltaTime > SERVER_RECIEVE_PRESENT_TIME)
			{
				PlayerID id = clientData.getID();
				m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
				send(id);
				removeClient(id);
				break;
			}
		}
}

ClientData* Server::getIDClientData(PlayerID requiredID)
{
	for (auto &cd : m_clientData)
		if (requiredID == cd.getID())
			return &cd;
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
	for (auto clientData : m_clientData)
		if (clientData.port == m_port)
			if(strcmp(clientData.ip, m_IP) == 0)
			return false;
	
	ClientData clientData = { 0 };
	PlayerID id = generateID();
	clientData.port = m_port;
	clientData.presentTime = m_pTimer->getCurrentTime();
	clientData.serverPlayer.initialize(id, m_pCpsIni->name, m_pGame);
	m_pSpsIni->id = clientData.getID();
	Space space = m_pMap->getRandomEmptySpace();
	clientData.serverPlayer.setX(space.v1.x);
	clientData.serverPlayer.setY(space.v1.y);
	strcpy(clientData.ip, m_IP);
	m_clientData.push_back(clientData);
	return true;
}

void Server::removeLastClient()
{
	removeClient(m_pCpsDisconnect->id);
	return;
}

void Server::removeClient(PlayerID id)
{
	uint8_t i = 0;
	for (int i = 0; i < m_clientData.size(); i++)
		if (m_clientData[i].getID() == id)
		{
			m_clientData.erase(std::next(m_clientData.begin(), i));
			m_pSpsDisconnect->packetType = PACKET_PLAYER_DISCONNECTED;
			m_pSpsDisconnect->id = id;
			post();
		}
}

PlayerID Server::getLastRecieverId()
{
	PlayerID id = INVALID_ID;
	for (auto clientData : m_clientData)
	{
		if (strcmp(clientData.ip , m_IP) == 0 && clientData.port == m_port)
		{
			id = clientData.getID();
			break;
		}
	}

	return id;
}
