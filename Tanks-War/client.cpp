#include "client.h"
#include "fileio.h"

Client::Client() : m_protocol(netNS::UDP), m_serverPlayers(0), m_state(UNCONNECTED)
{
	memset(m_map, 0, MAX_NAME_LEN);
	memset(m_mapHash, 0, MD5_LEN);
}


Client::~Client()
{
}

void Client::initialize(Map* map)
{
	m_pMap = map;
	m_clientInfo = FileIO::readClientInfo();
}

bool Client::connect()
{
	FileIO::createClientInfo(m_clientInfo);
	m_net.createClient(m_clientInfo.serverIP, m_clientInfo.serverPort, m_protocol);
	
	send(m_clientInfo.playerName);
	Sleep(500);
	recv(m_map);
	if (strlen(m_map) > 0)
	{
		m_state = CONNECTED;
		if (!m_pMap->isMapExist(m_map, m_mapHash))
		{
			m_state = MAP_NOT_FOUND;
			m_net.closeSocket();
			return false;
		}
		if(!m_pMap->load(m_map))
		{
			m_state = MAP_NOT_FOUND;
			m_net.closeSocket();
			return false;
		}

		recv(&m_serverPlayers, true);
	}
	else
	{
		m_state = UNCONNECTED;
		m_net.closeSocket();
		return false;
	}
	
	return true;
}

void Client::send(void * data, int& size)
{
	m_net.sendData(data, size, m_clientInfo.serverIP, m_port);
}

void Client::send(char* text)
{
	int len = strlen(text);
	send(text, len);
}

int Client::recv(void * data)
{
	int size = MAX_PACKET_SIZE;
	m_net.readData(data, size, m_clientInfo.serverIP, m_port);
	if (size > 0)
		return NET_RESPONSE;

	return NET_NORESPONSE;
}

void Client::recv(void * data, bool wait)
{
	do
	{
		if (recv(data) == NET_RESPONSE)
			break;
		if (wait)
			Sleep(200);

	} while (wait);
}