#include "server.h"
#include "..\common\fileio.h"

Server::Server() : m_serverPort(netNS::DEFAULT_PORT), m_state(SERVER_NOT_RUNNING)
{
	memset(m_IP, 0, netNS::IP_SIZE);
	memset(m_sIP, 0, netNS::IP_SIZE);
	memset(m_sData, 0, MAX_PACKET_SIZE);
	memset(m_rData, 0, MAX_PACKET_SIZE);
	m_pCpsIni =  (CpsIni*)&m_rData;
	m_pCpsPresent = (CpsPresent*)&m_rData;
	m_pCpsDisconnect = (CpsDisconnect*)&m_rData;
	m_pSpsIni = (SpsIni*)&m_sData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_sData;
	m_pSpsPlayerInitData = (SpsPlayersInitData*)&m_sData;
	m_pSpsDisconnect = (SpsDisconnect*)&m_sData;
	m_pPacketType = (PacketType*)&m_rData;
}

Server::~Server()
{
}

void Server::initialize(Map* map)
{
	m_pMap = map;
	ServerInfo info = FileIO::readServerInfo();
	m_serverPort = info.port;
	m_gamePlayers = info.players;
	strcpy(m_map, info.map);
}

void Server::update()
{
	if (isStarted())
	{
		//checkClients();
		if (recv())
		{
			//present();
			if (m_state == SERVER_WAITING)
				getClients();

			if (*m_pPacketType == PACKET_DISCONNECT)
			{
				removeLastClient();
			}
			else if (*m_pPacketType == PACKET_PLAYERS_EXIST)
			{
				m_pSpsPlayersExist->packetType = PACKET_PLAYERS_EXIST;
				m_pSpsPlayersExist->players = m_clientData.size();
				reply();
			}
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
	if (m_net.createServer(m_serverPort, netNS::UDP) == netNS::NET_OK)
	{
		m_state = SERVER_WAITING;
	}
}

void Server::getClients()
{
	if (m_clientData.size() >= m_gamePlayers)
		return;

	if (*m_pPacketType == PACKET_INI)
	{
		if (addClient())
			m_pSpsIni->accept = true;
		else
			m_pSpsIni->accept = false;

		m_pSpsIni->packetType = PACKET_INI;
		
		m_pSpsIni->gamePlayers = m_gamePlayers;
		strcpy(m_pSpsIni->map, m_pMap->getName());
		char map[MAX_PATH] = { 0 };
		m_pMap->getFullPath(map);
		m_pSpsIni->checksum = FileIO::getCRC32(map);
		memset(map, 0, strlen(map));
		reply();
		Sleep(1000);
		recv();
		if (*m_pPacketType != PACKET_START_SEASSON)
		{
			m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
			reply();
			removeLastClient();
		}
		
		postPlayersExist();
		postPlayersIniData();
	}
}

void Server::send(PlayerID id)
{
	int size = MAX_PACKET_SIZE;
	for (auto clientData : m_clientData)
	{
		if (clientData.id == id)
		{
			m_net.sendData(m_sData, size, clientData.ip, clientData.port);
			break;
		}
	}

	sbClear();
}

void Server::postPlayersExist()
{
	m_pSpsPlayersExist->packetType = PACKET_PLAYERS_EXIST;
	m_pSpsPlayersExist->players = m_clientData.size();
	post();
}

void Server::postPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_PLAYERS_INI_DATA;
	auto players = m_clientData.size();
	PlayerIniData* initData = (PlayerIniData*)&m_pSpsPlayerInitData->playerIniData;
	size_t dataSize = sizeof(ClientData)*players;
	memset(initData, 0, dataSize);
	uint8_t i = 0;
	for (auto clientData : m_clientData)
	{
		strcpy(initData[i].name, clientData.name);
		initData[i].id = clientData.id;
		i++;
	}

	post();
}

void Server::reply()
{
	int size = MAX_PACKET_SIZE;
	m_net.sendData(m_sData, size, m_IP, m_port);
}

void Server::post()
{
	int size = MAX_PACKET_SIZE;
	for (auto clientData : m_clientData)
		m_net.sendData(m_sData, size, clientData.ip, clientData.port);

	sbClear();
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
				return client.id;
		}

	return result;
}

bool Server::recv(bool wait)
{
	rbClear();
	bool result = false;
//	memset(m_IP, 0, netNS::IP_SIZE);

	do
	{
		int size = MAX_PACKET_SIZE;
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
	if (*m_pPacketType == PACKET_PRESENT)
	{
		for (auto& clientData : m_clientData)
			if (clientData.id == getLastRecieverId())
				clientData.presentTime = timeGetTime();
	}
}

void Server::checkClients()
{
	if(m_clientData.size()>0)
		for (auto clientData : m_clientData)
		{
			auto t = timeGetTime();
			if (timeGetTime() - clientData.presentTime > SERVER_RECIEVE_PRESENT_TIME)
			{
				PlayerID id = clientData.id;
				m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
				send(id);
				removeClient(id);
				break;
			}
		}
}

bool Server::addClient()
{
	for (auto clientData : m_clientData)
		if (clientData.port == m_port)
			if(strcmp(clientData.ip, m_IP) == 0)
			return false;
	
	ClientData clientData = { 0 };
	clientData.id = generateID();
	m_pSpsIni->id = clientData.id;
	clientData.port = m_port;
	clientData.presentTime = timeGetTime();
	strcpy(clientData.ip, m_IP);
	strcpy(clientData.name, m_pCpsIni->name);
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
	for (auto& clientData : m_clientData)
	{
		if (clientData.id == id)
		{
			m_clientData.erase(std::next(m_clientData.begin(), i));
			break;
		}

		i++;
	}

	postPlayersExist();
}

PlayerID Server::getLastRecieverId()
{
	PlayerID id = INVALID_ID;
	for (auto clientData : m_clientData)
	{
		if (strcmp(clientData.ip , m_IP) == 0 && clientData.port == m_port)
		{
			id = clientData.id;
			break;
		}
	}

	return id;
}
