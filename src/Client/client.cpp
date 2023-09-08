// client.cpp
// Author: abramann

#include "client.h"
#include "tankswar.h"
#include "..\common\timer.h"
#include "..\common\net.h"
#include "..\common\inlined.inl"

Client::Client()
{
	m_port = ::random(networkNS::START_CLIENT_PORT, networkNS::END_CLIENT_PORT);
}

Client::~Client()
{
}

void Client::initialize(TanksWar* pTW)
{
	m_pTW = pTW;
	m_pTimer = m_pTW->getTimer();
	m_pServerIP = pTW->getServerIP();
}

bool Client::connect(char* ip, Port port)
{
	m_serverPort = port;
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
	int64 end = m_pTimer->getCurrentTime() + waitTime;
	do
	{
		if (recv())
			return true;
		else
			Sleep(1);
	} while (end > m_pTimer->getCurrentTime());

	return false;
}