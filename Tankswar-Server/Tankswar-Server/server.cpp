#include "server.h"

Server::Server() : m_port(netNS::DEFAULT_PORT), m_protocol(netNS::UDP)
{
	memset(m_playerIP, 0, sizeof(m_playerIP));
	memset(m_playerPort, UNSPECIFIED_PORT, sizeof(m_playerPort));
	memset(m_data, 0, sizeof(m_data));
}

Server::~Server()
{
}

int Server::initialize()
{
	int result = m_net.createServer(m_port, m_protocol);
	if (result != netNS::NET_OK)
		return result;

	getClients();
	return 0;
}

void Server::getClients()
{
	unsigned short port;
	do
	{
		int size = 2;
		while(m_playerPort[0] != UNSPECIFIED_PORT)
		{
			size = 2;
			m_net.readData(m_data, size, m_playerIP, m_playerPort[1]);
			if (size > 0)
			{
				PlayerInfo p1Info;
				p1Info.name = 1;
				send<PlayerInfo>(&p1Info, 1);
				return;
			}

			Sleep(200);
		}

		m_net.readData(m_data, size, &m_playerIP[0], m_playerPort[0]);
		
		if (size > 0)
		{
			PlayerInfo p0Info;
			p0Info.name = 0;
			send<PlayerInfo>(&p0Info, 0);
		}
		Sleep(200);

	} while (true);
}
