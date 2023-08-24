// server.cpp
// Author: abramann

#include "server.h"
#include "tankswarServer.h"
#include "..\common\timer.h"

Server::Server() : m_pServerPort(0), m_pTimer(0)
{
	memset(m_rIP, 0, netNS::IP_SIZE);
	memset(m_sData, 0, networkNS::MAX_PACKET_SIZE);
	memset(m_rData, 0, networkNS::MAX_PACKET_SIZE);
}

Server::~Server()
{
}

void Server::initialize(TanksWarServer* pTKServer)
{
	m_pTKServer = pTKServer;
	m_pTimer = pTKServer->getTimer();
	m_pServerPort = m_pTKServer->getPort();
}

bool Server::start()
{
	auto create = m_net.createServer(*m_pServerPort, netNS::UDP);
	return (create == netNS::NET_OK);
}		

void Server::close()
{
	m_net.closeSocket();
}

void Server::send(const std::string ip, Port port, int size)
{
	int _size = size;
	m_net.sendData(m_sData, _size, ip.c_str(), port);
	sbClear();
}

bool Server::recv()
{
	rbClear();
	int size = networkNS::MAX_PACKET_SIZE;
	m_net.readData(m_rData, size, m_rIP, m_rPort);
	return (size > 0);
}

/*
 void Server::update()
{
	//checkClients();
	if (recv())
	{
		switch (*m_pPacketType)
		{
		case PACKET_CLIENTS_EXIST:
			replyPlayersExist();
			break;
		case PACKET_DISCONNECT:
			removeLastClient();
			break;
		case PACKET_PRESENT_CLIENT:
			present();
			break;
		case PACKET_CLIENT_ACT:
			applyPlayerAct();
			break;
		case PACKET_INI:
			getClients();
			break;
		}
	}
	
}

void Server::post(int size)
{
	for (size_t i = 0; i < m_pClientData.size(); i++)
		m_net.sendData(m_sData, size, m_pClientData[i]->getIP(), m_pClientData[i]->getPort());

	sbClear();
}

void Server::postPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_CLIENTS_INI_DATA;
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
	m_pSpsPlayerUpdate->packetType = PACKET_CLIENT_UPDATE;
	m_pSpsPlayerUpdate->playerUpdate[0] = getIDClientData(id)->serverPlayer.getPlayerUpdate();
	m_pSpsPlayerUpdate->playerUpdate[0].id = id;
	int size = sizeof(PacketType) + sizeof(PlayerUpdate);
	post(size);
}

void Server::postPlayersUpdate()
{
	m_pSpsPlayerUpdate->packetType = PACKET_CLIENTS_UPDATE;
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
	m_pSpsPlayersExist->packetType = PACKET_CLIENTS_EXIST;
	m_pSpsPlayersExist->players = static_cast<uint8>(m_pClientData.size());
	int size = sizeof(SpsPlayersExist);
	reply(size);
}

void Server::replyPlayersIniData()
{
	m_pSpsPlayerInitData->packetType = PACKET_CLIENTS_INI;
	int8 connectedClients = static_cast<int8>(m_pClientData.size());
	for (int i = 0; i < connectedClients; i++)
	{
		m_pSpsPlayerInitData->playerIniData[i].id = m_pClientData[i]->getID();
		strcpy_s(m_pSpsPlayerInitData->playerIniData[i].name, m_pClientData[i]->getName());
	}

	int size = sizeof(PacketType) + (sizeof(PlayerIniData) * connectedClients);
	reply(size);
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

void Server::removeLastClient()
{
	removeClient(m_pCpsDisconnect->id);
}

PlayerID Server::getLastRecieverId()
{
	PlayerID id = 0;
	/*for (auto pClientData : m_pClientData)
	{
		if (strcmp(pClientData->getIP(), m_IP) == 0 && pClientData->getPort() == m_port)
		{
			id = pClientData->getID();
			break;
		}
	}

	return id;
}
*/