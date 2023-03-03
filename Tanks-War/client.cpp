#include "client.h"

Client::Client() : m_protocol(netNS::UDP), m_serverPort(netNS::DEFAULT_PORT)
{
}


Client::~Client()
{
}

int Client::initialize(char * serverIP, char* playerName, uint8_t& players)
{
	strcpy(m_serverIP, serverIP);
	int result = m_net.createClient(m_serverIP, m_serverPort, m_protocol);
	send(playerName);
	recv(&players, true);
	return result;
}

void Client::send(void * data, int& size)
{
	m_net.sendData(data, size, m_serverIP, m_port);
}

void Client::send(char* text)
{
	int len = strlen(text);
	send(text, len);
}

int Client::recv(void * data)
{
	int size = MAX_PACKET_SIZE;
	m_net.readData(data, size, m_serverIP, m_port);
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