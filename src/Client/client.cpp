// client.cpp
// Author: abramann

#include "client.h"
#include "tankswar.h"
#include "..\common\net.h"

Client::Client()
{
	m_port = _rand(3000);
}

Client::~Client()
{
}

void Client::initialize(TanksWar* pTW)
{
	m_pTW = pTW;
	m_pServerIP = pTW->getServerIP();
}

bool Client::connect(char* ip, Port port)
{
	m_serverPort = m_serverPort = port;
	m_pServerIP = ip;
	auto create = m_net.createClient(m_pServerIP, m_serverPort, netNS::UDP);
	return (create == netNS::NET_OK);
}

void Client::disconnect()
{
	m_net.closeSocket();
}

void Client::send(int size)
{
	m_net.sendData(m_sData, size, m_pServerIP, m_port);
	sbClear();
}

bool Client::recv()
{
	rbClear();
	int size = networkNS::MAX_PACKET_SIZE;
	m_net.readData(m_rData, size, m_pServerIP, m_port);
	return (size > 0) ? true : false;
}

bool Client::recv(int64 waitTime)
{
	do
	{
		if (recv())
			return true;
		else
			Sleep(waitTime--);
	} while (waitTime > 0);

	return false;
}