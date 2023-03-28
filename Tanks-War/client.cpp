#include "client.h"
#include "fileio.h"

Client::Client() : m_connectedPlayers(0), m_gamePlayers(0), m_state(CLIENT_UNCONNECTED), m_presentTime(0)
{
	memset(m_map, 0, MAX_NAME_LEN);
	m_pCpsIni = (CpsIni*)&m_sData;
	m_pCpsDisconnect = (CpsDisconnect*)&m_sData;
	m_pCpsPresent = (CpsPresent*)&m_sData;
	m_pCpsSeasson = (CpsSeasson*)&m_sData;
	m_pSpsIni = (SpsIni*)&m_rData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_rData;
	m_pSpsPlayerIniData = (SpsPlayersInitData*)&m_rData;
	m_pPacketType = (PacketType*)&m_rData;
	m_port = _rand(3000);
}

Client::~Client()
{
}

void Client::initialize(Map* map)
{
	m_pMap = map;
	m_clientInfo = FileIO::readClientInfo();
}

void Client::update()
{
	//if (isConnected())
	//	present();

	if (recv())
	{
		m_port;
		if (m_state == CLIENT_CONNECTED_WAITING)
			wait();

		switch (*m_pPacketType)
		{
		case PACKET_PLAYERS_EXIST:
			m_connectedPlayers = m_pSpsPlayersExist->players;
			break;
		case PACKET_DISCONNECT:
			disconnect();
			break;
		case PACKET_PLAYERS_INI_DATA:
			initPlayers();
			break;
		default:
			break;
		}
	}
}

bool Client::connect()
{
	FileIO::createClientInfo(m_clientInfo);
	m_net.createClient(m_clientInfo.serverIP, m_clientInfo.serverPort, netNS::UDP);
	m_pCpsIni->packetType = PACKET_INI;
	strcpy(m_pCpsIni->name, m_clientInfo.name);
	m_port = _rand(MAX_PORT);
	send();
	Sleep(500);
	if (recv() && *m_pPacketType == PACKET_INI)
	{
		strcpy(m_map, m_pSpsIni->map);
		if (!m_pMap->isMapExist(m_map, m_pSpsIni->checksum))
		{
			m_state = CLIENT_UNCONNECTED_MAP_NOT_FOUND;
			m_net.closeSocket();
			return false;
		}
		
		m_pCpsSeasson->packetType = PACKET_START_SEASSON;
		send();
		m_state = CLIENT_CONNECTED_WAITING;
		m_gamePlayers = m_pSpsIni->gamePlayers;
		m_id = m_pSpsIni->id;
		if (!m_pMap->load(m_map))
		{
			disconnect();
			m_state = CLIENT_UNCONNECTED_MAP_NOT_LOAD;
			return false;
		}
	}
	
	return true;
}

void Client::present()
{
	if (timeGetTime() - m_presentTime < CLIENT_PRESENT_TIME)
		return;

	m_pCpsPresent->packet = PACKET_PRESENT;
	m_pCpsPresent->id = m_id;
	send();
	m_presentTime = timeGetTime();
}

void Client::disconnect()
{
	m_pCpsDisconnect->packetType = PACKET_DISCONNECT;
	m_pCpsDisconnect->id = m_id;
	send();
	m_state = CLIENT_UNCONNECTED_DISCONNECT;
	m_net.closeSocket();
}

void Client::wait()
{

	if (*m_pPacketType == PACKET_PLAYERS_INI_DATA)
	{
		m_state = CLIENT_CONNECTED_PLAYING;
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

void Client::initPlayers()
{
	m_pSpsPlayerIniData->playerIniData->id;
	for (int i = 0; i < m_connectedPlayers; i++)
		m_playerData.push_back(m_pSpsPlayerIniData->playerIniData[i]);
}