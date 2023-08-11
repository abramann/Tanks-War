// server.cpp
// Author: abramann

#include "server.h"
#include "..\common\fileio.h"
#include "..\common\game.h"
#include "..\common\timer.h"
#include "..\common\map.h"
#include "..\common\texturemanger.h"

Server::Server() : m_serverPort(netNS::DEFAULT_PORT), m_state(SERVER_NOT_RUNNING), m_pAudio(0), m_pGraphics(0),
m_pTimer(0), m_pTextureManger(0), m_pMap(0)
{
	memset(m_IP, 0, netNS::IP_SIZE);
	memset(m_sIP, 0, netNS::IP_SIZE);
	memset(m_sData, 0, networkNS::MAX_PACKET_SIZE);
	memset(m_rData, 0, networkNS::MAX_PACKET_SIZE);
	m_pCpsIni = (CpsIni*)&m_rData;
	m_pCpsPresent = (CpsPresent*)&m_rData;
	m_pCpsDisconnect = (CpsDisconnect*)&m_rData;
	m_pCpsPlayerAct = (CpsPlayerAct*)&m_rData;
	m_pSpsIni = (SpsIni*)&m_sData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_sData;
	m_pSpsPlayerInitData = (SpsPlayersInitData*)&m_sData;
	m_pSpsDisconnect = (SpsDisconnect*)&m_sData;
	m_pSpsPlayerUpdate = (SpsPlayerUpdate*)&m_sData;
	m_pSpsPlayerAct = (SpsPlayerAct*)&m_sData;
	m_pPacketType = (PacketType*)&m_rData;
}

Server::~Server()
{
	if (isStarted())
		stop();
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
		pClientData->update();

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
	if (m_pClientData.size() >= m_gameMaxPlayers)
		return;
	if (!addClient())
		return;

	PlayerID id = m_pSpsIni->id;
	m_pSpsIni->packetType = PACKET_INI;
	m_pSpsIni->gamePlayers = m_gameMaxPlayers;
	strcpy_s(m_pSpsIni->map, m_pMap->getMap());
	m_pSpsIni->checksum = m_pMap->getCrc32();
	int size = sizeof(SpsIni);
	reply(size);
	postNewPlayer();
	replyPlayersExist();
	replyPlayersIniData();
	postPlayersUpdate();
	//postPlayerUpdate(id);
}

void Server::send(PlayerID id, int size)
{
	int s = size;
	for (auto pClientData : m_pClientData)
	{
		if (pClientData->getID() == id)
		{
			m_net.sendData(m_sData, s, pClientData->getIP(), pClientData->getPort());
			break;
		}
	}

	sbClear();
}

void Server::post(int size)
{
	for (size_t i = 0; i < m_pClientData.size(); i++)
		m_net.sendData(m_sData, size, m_pClientData[i]->getIP(), m_pClientData[i]->getPort());

	sbClear();
}

void Server::postPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI_DATA;
	auto players = m_pClientData.size();
	PlayerIniData* initData = (PlayerIniData*)&m_pSpsPlayerInitData->playerIniData;
	size_t dataSize = sizeof(ClientData)*players;
	int connectedClients = m_pClientData.size();
	memset(initData, 0, dataSize);
	for (int i = 0; i < connectedClients; i++)
	{
		strcpy_s(initData[i].name, m_pClientData[i]->getName());
		initData[i].id = m_pClientData[i]->getID();
	}

	int size = sizeof(SpsPlayersInitData) * connectedClients;
	//	size = 255;
	post(size);
}

void Server::postPlayerUpdate(PlayerID id)
{
	m_pSpsPlayerUpdate->packetType = PACKET_PLAYER_UPDATE;
	m_pSpsPlayerUpdate->playerUpdate[0] = getIDClientData(id)->serverPlayer.getPlayerUpdate();
	m_pSpsPlayerUpdate->playerUpdate[0].id = id;
	int size = sizeof(PacketType) + sizeof(PlayerUpdate);
	post(size);
}

void Server::postPlayersUpdate()
{
	m_pSpsPlayerUpdate->packetType = PACKET_PLAYERS_UPDATE;
	int connectedClients = m_pClientData.size();
	for (int i = 0; i < connectedClients; i++)
	{
		m_pSpsPlayerUpdate->playerUpdate[i] = m_pClientData[i]->serverPlayer.getPlayerUpdate();
		m_pSpsPlayerUpdate->playerUpdate[i].id = m_pClientData[i]->getID();
	}

	int size = sizeof(PacketType) + sizeof(PlayerUpdate) * connectedClients;
	//size = 255;
	post(size);
}

void Server::postNewPlayer()
{
	m_pSpsPlayerInitData->packetType = PACKET_NEW_PLAYER;
	m_pSpsPlayerInitData->playerIniData->id = m_pClientData.back()->getID();
	strcpy_s(m_pSpsPlayerInitData->playerIniData->name, m_pClientData.back()->getName());
	int size = sizeof(PacketType) + sizeof(PlayerIniData);
	post(size);
}

void Server::reply(int size)
{
	int s = size;
	m_net.sendData(m_sData, s, m_IP, m_port);
	sbClear();
}

void Server::replyPlayersExist()
{
	m_pSpsPlayersExist->packetType = PACKET_PLAYERS_EXIST;
	m_pSpsPlayersExist->players = static_cast<uint8>(m_pClientData.size());
	int size = sizeof(SpsPlayersExist);
	reply(size);
}

void Server::replyPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI;
	int8 connectedClients = static_cast<int8>(m_pClientData.size());
	for (int i = 0; i < connectedClients; i++)
	{
		m_pSpsPlayerInitData->playerIniData[i].id = m_pClientData[i]->getID();
		strcpy_s(m_pSpsPlayerInitData->playerIniData[i].name, m_pClientData[i]->getName());
	}

	int size = sizeof(PacketType) + (sizeof(PlayerIniData) * connectedClients);
	reply(size);
}

void Server::clearClients()
{
	m_pClientData.clear();
}

PlayerID Server::recvID(bool wait)
{
	bool result = false;
	Port port = 0;
	bool recieved = recv(wait);
	if (recieved)
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
		int size = networkNS::MAX_PACKET_SIZE;
		m_net.readData(m_rData, size, m_IP, m_port);
		if (size > 0)
		{
			result = true;
			break;
		}
	} while (wait);

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
			if (deltaTime > networkNS::SERVER_RECIEVE_PRESENT_TIME)
			{
				PlayerID id = pClientData->getID();
				m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
				int size = sizeof(SpsDisconnect);
				send(id, size);
				removeClient(id);
				break;
			}
		}
}

ClientData* Server::getIDClientData(PlayerID requiredID)
{
	ClientData* pClientData = 0;
	for (auto &cd : m_pClientData)
		if (requiredID == cd->getID())
		{
			pClientData = cd.get();
			break;
		}

	return pClientData;
}

void Server::applyPlayerAct()
{
	PlayerID& id = m_pCpsPlayerAct->id;
	ServerPlayer& serverPlayer = getIDClientData(id)->serverPlayer;
	PlayerAct& act = m_pCpsPlayerAct->act;
	if (act >= PLAYER_ACT_ATTACK)
	{
		act -= PLAYER_ACT_ATTACK;
		if (!serverPlayer.isBulletLaunching())
		{
			serverPlayer.executeAttack();
			m_pSpsPlayerAct->packetType = PACKET_PLAYER_ACT;
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

	postPlayerUpdate(id);
}

bool Server::addClient()
{
	for (auto pClientData : m_pClientData)
		if (pClientData->getPort() == m_port)
			if (strcmp(pClientData->getIP(), m_IP) == 0)
				return false;

	PlayerID id = generateID();
	m_pSpsIni->id = id;
	std::shared_ptr<ClientData> pClientData = std::make_shared<ClientData>(id, m_pCpsIni->name, m_IP, m_port, m_pGame);
	Space space = m_pMap->getRandomEmptySpace();
	V3 position = V3(space.v1.x, space.v1.y, 0);
	pClientData->serverPlayer.setPosition(position);
	m_pClientData.push_back(pClientData);
	return true;
}

void Server::removeLastClient()
{
	removeClient(m_pCpsDisconnect->id);
}

void Server::removeClient(PlayerID id)
{
	for (size_t i = 0; i < m_pClientData.size(); i++)
		if (m_pClientData[i]->getID() == id)
		{
			m_pClientData.erase(std::next(m_pClientData.begin(), i));
			m_pSpsDisconnect->packetType = PACKET_PLAYER_DISCONNECTED;
			m_pSpsDisconnect->id = id;
			int size = sizeof(SpsDisconnect);
			post(size);
			break;
		}
}

PlayerID Server::getLastRecieverId()
{
	PlayerID id = 0;
	for (auto pClientData : m_pClientData)
	{
		if (strcmp(pClientData->getIP(), m_IP) == 0 && pClientData->getPort() == m_port)
		{
			id = pClientData->getID();
			break;
		}
	}

	return id;
}