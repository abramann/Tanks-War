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

void Server::send(const std::string ip, Port port, int32 size, bool bufferClear)
{
	int32 _size = size;
	m_net.sendData(m_sData, _size, ip.c_str(), port);
	if (bufferClear)
		sbClear();
}

bool Server::recv()
{
	rbClear();
	int32 size = networkNS::MAX_PACKET_SIZE;
	m_net.readData(m_rData, size, m_rIP, m_rPort);
	return (size > 0);
}