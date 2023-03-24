#include "client.h"
#include "fileio.h"

Client::Client() : m_serverPlayers(0), m_state(CLIENT_UNCONNECTED)
{
	memset(m_map, 0, MAX_NAME_LEN);
	m_pCpsIni = (CpsIni*)&m_sData;
	m_pCpsDisconnect = (CpsDisconnect*)&m_sData;
	m_pCpsSeasson = (CpsSeasson*)&m_sData;
	m_pSpsIni = (SpsIni*)&m_rData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_rData;
	m_pSpsPlayerIniData = (SpsPlayersIniData*)&m_rData;
	m_pPacketType = (PacketType*)&m_rData;
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
	m_net.createClient(m_clientInfo.serverIP, m_clientInfo.serverPort, netNS::UDP);
	m_pCpsIni->packetType = PACKET_INI;
	strcpy(m_pCpsIni->name, m_clientInfo.name);
	send();
	Sleep(500);
	if (recv() && *m_pPacketType == PACKET_INI)
	{
		strcpy(m_map, m_pSpsIni->map);
		if (!m_pMap->isMapExist(m_map, m_pSpsIni->checksum))
		{
			m_state = CLIENT_MAP_NOT_FOUND;
			m_net.closeSocket();
			return false;
		}
		
		m_pCpsSeasson->packetType = PACKET_START_SEASSON;
		send();
		m_state = CLIENT_WAITING;
		m_serverPlayers = m_pSpsIni->playersInServer;
		m_gamePlayers = m_pSpsIni->gamePlayers;
		m_id = m_pSpsIni->id;
		if (!m_pMap->load(m_map))
		{
			disconnect();
			m_state = CLIENT_MAP_NOT_LOAD;
			return false;
		}
	}
	
	return true;
}

void Client::disconnect()
{
	m_pCpsDisconnect->packetType = PACKET_DISCONNECT;
	send();
	m_state = CLIENT_DISCONNECTED;
	m_net.closeSocket();
}

void Client::wait()
{
	recv();
	if (*m_pPacketType == PACKET_PLAYERS_EXIST)
	{
		m_serverPlayers = m_pSpsPlayersExist->players;
	}
	else if (*m_pPacketType == PACKET_PLAYERS_INI_DATA)
	{
		m_state = CLIENT_RUNNING;
		
	}
}

void Client::send()
{
	int size = MAX_PACKET_SIZE;
	m_net.sendData(m_sData, size, m_clientInfo.serverIP, m_port);
	sbClear();
}

bool Client::recv()
{
	rbClear();
	int size = 256;
	m_net.readData(m_rData, size, m_clientInfo.serverIP, m_port);
	return (size > 0) ? true : false;
}

void Client::recv(bool wait)
{
	do
		Sleep(200);
	while (recv() != false && wait);
}