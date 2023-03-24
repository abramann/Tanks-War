#include "server.h"
#include "..\common\fileio.h"
#include <memory>

Server::Server() : m_serverPort(netNS::DEFAULT_PORT), m_clients(0), m_state(SERVER_NOT_RUNNING), m_id(0)
{
	memset(m_IP, 0, netNS::IP_SIZE);
	memset(m_sIP, 0, netNS::IP_SIZE);
	memset(m_sData, 0, MAX_PACKET_SIZE);
	memset(m_rData, 0, MAX_PACKET_SIZE);
	m_pCpsIni =  (CpsIni*)&m_rData;
	m_pSpsIni = (SpsIni*)&m_sData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_sData;
	m_pSpsPlayerIniData = (SpsPlayersIniData*)&m_sData;
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
	if ( (m_state == SERVER_HANDLING || m_state==SERVER_WAITING ) && recv())
	{
		if (m_state == SERVER_WAITING)
			getClients();

		if (*m_pPacketType == PACKET_DISCONNECT)
		{
			removeClient();
			m_pSpsPlayersExist->packetType = PACKET_PLAYERS_EXIST;
			m_pSpsPlayersExist->players = m_clients;
			post();
		}
		else if (*m_pPacketType == PACKET_PLAYERS_EXIST)
		{
			m_pSpsPlayersExist->packetType = PACKET_PLAYERS_EXIST;
			m_pSpsPlayersExist->players = m_clients;
			reply();
		}
	}	
}

void Server::stop()
{
	m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
	post();
	m_net.closeSocket();
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
	if (*m_pPacketType == PACKET_INI)
	{
		if (addClient())
			m_pSpsIni->accept = true;
		else
			m_pSpsIni->accept = false;

		m_pSpsIni->packetType = PACKET_INI;
		m_pSpsIni->playersInServer = m_clients;
		
		m_pSpsIni->gamePlayers = m_gamePlayers;
		strcpy(m_pSpsIni->map, m_pMap->getName());
		m_pSpsIni->id = m_id;
		char map[MAX_PATH] = { 0 };
		strcpy(map,(m_pMap->getFullPath()));
		m_pSpsIni->checksum = FileIO::getCRC32(map);
		memset(map, 0, strlen(map));
		reply();
		Sleep(1000);
		recv();
		if (*m_pPacketType != PACKET_START_SEASSON)
			removeClient();
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

bool Server::addClient()
{
	for (auto clientData : m_clientData)
		if (strcmp(clientData.ip, m_IP) == 0 && clientData.port == m_port)
			return false;

	ClientData clientData = { 0 };
	clientData.id = m_id++;
	clientData.port = m_port;
	strcpy(clientData.ip, m_IP);
	strcpy(clientData.name, m_pCpsIni->name);
	m_clientData.push_back(clientData);
	m_clients++;
	return true;
}

void Server::removeClient()
{
	int i = 0;
	for (auto clientData : m_clientData)
	{
	//	auto clientData = m_clientData[i];
		if (strcmp(clientData.ip, m_IP) == 0 && clientData.port == m_port)
		{
			m_clientData.erase(std::next(m_clientData.begin() , i));
			if (m_clients > 0)	m_clients--;
			break;
		}
		i++;
	}
}

PlayerID Server::getLastRecieverId()
{
	PlayerID id = INVALID_ID;
	for (auto clientData : m_clientData)
	{
		if (clientData.ip == m_IP && clientData.port == m_port)
		{
			id = clientData.id;
			break;
		}
	}

	return id;
}
