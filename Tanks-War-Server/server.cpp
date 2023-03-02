#include "server.h"

Server::Server() : m_serverPort(netNS::DEFAULT_PORT), m_protocol(netNS::UDP), m_players(0)
{
}

Server::~Server()
{
}

int Server::initialize(uint8_t players)
{
	m_players = players;
	m_pPlayerIP = new char[m_players*netNS::IP_SIZE];
	memset(m_pPlayerIP, 0, m_players*netNS::IP_SIZE);
	m_pPlayerPort = new Port[m_players];
	memset(m_pPlayerPort, 0, m_players*sizeof(Port));
	m_pPlayerInfo = new PlayerInfo[m_players];
	memset(m_pPlayerInfo, 0, m_players*sizeof(PlayerInfo));

	int result = m_net.createServer(m_serverPort, m_protocol);
	if (result != netNS::NET_OK)
		return result;

	recvPlayerResponse();
	return 0;
}

void Server::getClients()
{
	char name[MAX_PLAYER_NAME] = { 0 };
	char ip[netNS::IP_SIZE] = { 0 };
	Port port;
	for (int i = 0; i < m_players; i++)
	{
		recv(name, ip, port, true);
		
	}

}

void Server::send(void * data, int size, PlayerID id)
{
	int _size = size;
	m_net.sendData(data, size, &m_pPlayerIP[id*netNS::IP_SIZE], m_pPlayerPort[id]);
}

PlayerID Server::recv(void * data, bool wait)
{
	PlayerID id = INVALID_ID;
	Port port;
	char ip[netNS::IP_SIZE] = { 0 };
	recv(data, ip, port, wait);
	for (int i = 0; i < m_players; i++)
		if (m_pPlayerPort[i] == port)
		{
			id = i;
			break;
		}

	return id;
}
