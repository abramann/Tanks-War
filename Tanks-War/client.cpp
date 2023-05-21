#include "client.h"
#include "fileio.h"
#include "game.h"
#include "net.h"
#include "tank2.h"

Client::Client() : m_gamePlayers(0), m_state(CLIENT_UNCONNECTED), m_presentTime(0)
{
	memset(m_map, 0, MAX_NAME_LEN);
	m_pCpsIni = (CpsIni*)&m_sData;
	m_pCpsDisconnect = (CpsDisconnect*)&m_sData;
	m_pCpsPresent = (CpsPresent*)&m_sData;
	m_pCpsSeasson = (CpsSeasson*)&m_sData;
	m_pCpsPlayerAct = (CpsPlayerAct*)&m_sData;
	m_pSpsIni = (SpsIni*)&m_rData;
	m_pSpsPlayersExist = (SpsPlayersExist*)&m_rData;
	m_pSpsPlayerIniData = (SpsPlayersInitData*)&m_rData;
	m_pSpsPlayerUpdate = (SpsPlayerUpdate*)&m_rData;
	m_pPacketType = (PacketType*)&m_rData;
	m_port = _rand(3000);
	m_pClientPlayer = std::make_shared<ClientPlayer>();
}
Client::~Client()
{
}

void Client::initialize(Game* game)
{
	m_pGame = game;
	m_pMap = game->getMap();
	m_pTextureManger = game->getTextureManger();
	m_pAudio = game->getAudio();
	m_pGraphics = game->getGraphics();
	m_pInput = game->getInput();
	m_pTimer = game->getTimer();
	m_clientInfo = FileIO::readClientInfo();
}

void Client::update()
{
	//if (isConnected())
	//	present();
	if (m_state == CLIENT_CONNECTED_PLAYING)
	{
		m_pClientPlayer->update(0);
		PlayerAct act = m_pClientPlayer->getAct();
		if (act != PLAYER_ACT_NONE)
		{
			m_pCpsPlayerAct->packetType = PACKET_PLAYER_ACT;
			m_pCpsPlayerAct->act = act;
			m_pCpsPlayerAct->id = m_pClientPlayer->getID();
			send();
		}

		for (auto& clientData : m_clientData)
		{
			ServerPlayer& serverPlayer = clientData.serverPlayer;
			serverPlayer.update(0);
		}
	}
	if (recv())
	{
		switch (*m_pPacketType)
		{
		case PACKET_DISCONNECT:
			disconnect();
			break;
		case PACKET_PLAYERS_INI_DATA:
			initPlayers();
			break;
		case PACKET_PLAYER_UPDATE:
			playerUpdate();
			break;
		case PACKET_PLAYERS_UPDATE:
			playersUpdate();
			break;
		case PACKET_NEW_PLAYER:
			addNewPlayer();
			break;
		case PACKET_PLAYER_DISCONNECTED:
			removeClient(m_pCpsDisconnect->id);
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

		//	m_pCpsSeasson->packetType = PACKET_START_SEASSON;
		//	send();
		m_state = CLIENT_CONNECTED_PLAYING;
		m_gamePlayers = m_pSpsIni->gamePlayers;
		m_pClientPlayer->initialize(m_pSpsIni->id, m_pGame);
		if (!m_pMap->load(m_map))
		{
			disconnect();
			m_state = CLIENT_UNCONNECTED_MAP_NOT_LOAD;
			return false;
		}
		
		uint8 players = 0;
		do
		{
			recv(true);
			if (*m_pPacketType == PACKET_PLAYERS_EXIST)
				players = m_pSpsPlayersExist->players;
		} while (*m_pPacketType != PACKET_PLAYERS_INI);

		for (int i = 0; i < players; i++)
		{
			PlayerIniData& iniData = m_pSpsPlayerIniData->playerIniData[i];
			if (iniData.id == m_pClientPlayer->getID())
				continue;

			ClientData clientData;
			clientData.serverPlayer.initialize(iniData.id, iniData.name, m_pGame);
			m_clientData.push_back(clientData);
		}
	}

	return true;
}

void Client::present()
{
	auto timeDelta = m_pTimer->getCurrentTime() - m_presentTime;
	if (timeDelta < CLIENT_PRESENT_TIME * 1000)
		return;

	m_pCpsPresent->packet = PACKET_PRESENT_CLIENT;
//	m_pCpsPresent->id = m_id;
	send();
	m_presentTime = m_pTimer->getCurrentTime();
}

void Client::disconnect()
{
	m_pCpsDisconnect->packetType = PACKET_DISCONNECT;
	m_pCpsDisconnect->id = m_pClientPlayer->getID();
	send();
	m_state = CLIENT_UNCONNECTED_DISCONNECT;
	m_net.closeSocket();
	m_clientData.clear();
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
	{
		if (recv())
			wait = false;
		else
			Sleep(100);
	} while (wait);
}

void Client::initPlayers()
{
}

void Client::addNewPlayer()
{
	PlayerID& id = m_pSpsPlayerIniData->playerIniData->id;
	const char* name = m_pSpsPlayerIniData->playerIniData->name;
	ClientData clientData(id, name, m_pGame);
	m_clientData.push_back(clientData);
	recv(true);
	playerUpdate();
}

void Client::removeClient(PlayerID id)
{
	for (int i = 0; i < m_clientData.size(); i++)
	{
		if (m_clientData[i].getID() == id);
		m_clientData.erase(std::next(m_clientData.begin(), i));
		break;
	}
}

inline ClientData* getIDClient(const PlayerID id, std::vector<ClientData>* clientData)
{
	ClientData* idClient = 0;;
	for (int i = 0; i < clientData->size(); i++)
	{
		if (clientData->at(i).getID() == id)
		{
			idClient = &clientData->at(i);
			break;
		}
	}

	return idClient;
}

void Client::playerUpdate()
{
	PlayerID& id = m_pSpsPlayerUpdate->playerUpdate->id;
	ClientData& clientData = *getIDClient(id, &m_clientData);
//	clientData.playerTank.applyPlayerUpdate(*m_pSpsPlayerUpdate->playerUpdate);
}

void Client::playersUpdate()
{
	PlayerUpdate* playerUpdate = m_pSpsPlayerUpdate->playerUpdate;

	for (int i = 0; i < m_clientData.size() + 1; i++)
	{
		PlayerID& id = playerUpdate[i].id;
		if (m_pClientPlayer->getID() == id)
			m_pClientPlayer->applyPlayerUpdate(playerUpdate[i]);
		else
		{
			ServerPlayer& serverPlayer = getIDClient(id, &m_clientData)->serverPlayer;
			serverPlayer.applyPlayerUpdate(playerUpdate[i]);
		}
	}
}