// tankswarserver.cpp
// Author: abramann

#include "tankswarserver.h"
#include "..\common\fileio.h"
#include "..\common\texturemanger.h"
#include "..\common\interface.h"
#include "..\common\map.h"

TanksWarServer::TanksWarServer()
{
	m_pServer = std::make_shared<Server>();
	
}

TanksWarServer::~TanksWarServer()
{
}

void TanksWarServer::initialize(HINSTANCE hInstance, HWND hwnd)
{
	Game::initialize(hInstance, hwnd);
	m_pInterface->initialize(m_pServer.get(), this);
	m_pServer->initialize(this);
	m_pRData = m_pServer->getReceiveBuffer();
	m_pSData = m_pServer->getSendBuffer();
	m_pReceiverIP = m_pServer->getReceiverIP();
	m_pReceiverPort = m_pServer->getReceiverPort();
	m_pPacketType = (PacketType*)m_pRData;

	m_pCpsJoin = (CpsJoin*)m_pRData;
	m_pCpsHeartbeat = (CpsHeartbeat*)m_pRData;
	m_pCpsDisconnect = (CpsDisconnect*)m_pRData;

	m_pSpsDisconnect = (SpsDisconnect*)m_pSData;
	m_pSpsJoin = (SpsJoin*)m_pSData;
	m_pSpsClientGameState = (SpsClientGameState*)m_pSData;
}

void TanksWarServer::update()
{
}

void TanksWarServer::updateGameState()
{
}

void TanksWarServer::render()
{
	m_pInterface->render();
}

void TanksWarServer::communicate()
{
	if (m_pClient.size() < m_gameMaxPlayers)
		handleNewClient();
}

void TanksWarServer::handleNewClient()
{
	if (*m_pPacketType == PACKET_CLIENT_JOIN)
		if (clientExist())
			return;

	createClient();
}

void TanksWarServer::createClient()
{
	PlayerID uniqueID = generateClientID();
	auto pClient = make_shared<Client>(uniqueID, *m_pCpsJoin->name, m_pReceiverIP);
	m_pSpsJoin->id = uniqueID;
	strcpy(m_pSpsJoin->name, m_pSpsJoin->name);
	post(sizeof(m_pSpsJoin));
	resetClientGameState(pClient.get());
}

bool TanksWarServer::clientExist()
{
	for (auto pClient : m_pClient)
		if (strcmp(m_pReceiverIP, pClient->ip) == 0 && *m_pReceiverPort == pClient->port)
			return true;

	return false;
}

void TanksWarServer::receiveClientHeartbeat()
{
	/*if (*m_pPacketType == PACKET_PRESENT_CLIENT)
	{
		for (auto& pClientData : m_pClientData)
			if (pClientData->getID() == getLastRecieverId())
				pClientData->presentTime = m_pTimer->getCurrentTime();
	}*/
}

void TanksWarServer::disconnectClient(Client* pClient)
{
	auto it = std::find(m_pClient.begin(), m_pClient.end(), pClient);
	m_pSpsDisconnect->packetType = PACKET_CLIENT_DISCONNECTED;
	m_pSpsDisconnect->id = pClient->serverPlayer.getID();
	m_pClient.erase(it);
	post(sizeof(SpsDisconnect));
}

void TanksWarServer::disonnectInactiveClient()
{
	/*if (m_pClientData.size() > 0)
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
		}*/
}

void TanksWarServer::serverStart()
{
	m_pServer->start();
	
}

void TanksWarServer::serverShutdown()
{
	m_pSpsDisconnect->packetType = PACKET_DISCONNECT;
	post();
	m_pServer->close();
	m_pClient.clear();
}

void TanksWarServer::applyPlayerMove()
{
	/*PlayerID& id = m_pCpsPlayerAct->id;
	ServerPlayer& serverPlayer = getIDClientData(id)->serverPlayer;
	PlayerAct& act = m_pCpsPlayerAct->act;
	if (act >= PLAYER_ACT_ATTACK)
	{
		act -= PLAYER_ACT_ATTACK;
		if (!serverPlayer.isBulletLaunching())
		{
			serverPlayer.executeAttack();
			m_pSpsPlayerAct->packetType = PACKET_CLIENT_ACT;
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

	postPlayerUpdate(id);*/
}

void TanksWarServer::post(int size)
{
	for (auto pClient : m_pClient)
		send(pClient.get(), size);
}

void TanksWarServer::send(Client* pClient, int size)
{
	m_pServer->send(pClient->ip, pClient->port, size);
}

void TanksWarServer::resetClientGameState(Client* pClient)
{
	Space space = m_pMap->getRandomEmptySpace();
	V3 position = V3(space.v1.x, space.v1.y, 0);
	pClient->serverPlayer.setPosition(position);
	updateClientGameState(pClient);
}

void TanksWarServer::updateClientGameState(Client* pClient)
{
	m_pSpsClientGameState->packetType = PACKET_CLIENT_GAME_STATE;
	m_pSpsClientGameState->id = pClient->serverPlayer.getID();
	m_pSpsClientGameState->position = pClient->serverPlayer.getPosition();
	m_pSpsClientGameState->rotate = pClient->serverPlayer.getRotate();
	m_pSpsClientGameState->health = pClient->serverPlayer.getHealth();
	m_pSpsClientGameState->velocity = pClient->serverPlayer.getVelocity();
	post(sizeof(SpsClientGameState));
}

PlayerID TanksWarServer::generateClientID() const
{
	static std::vector<PlayerID> clientID;
	PlayerID id = 0;
	do
		id = _rand(255);
	while ((std::find(clientID.begin(), clientID.end(), id) != clientID.end());

	clientID.push_back(id);
	return id;
}
