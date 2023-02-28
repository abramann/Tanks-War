#include "client.h"

Client::Client() : m_protocol(netNS::UDP), m_port(netNS::DEFAULT_PORT)
{
}


Client::~Client()
{
}

int Client::initialize(char * serverIP)
{
	strcpy(m_serverIP, serverIP);
	return m_net.createClient(m_serverIP, m_port, m_protocol);
}

int Client::send(void * data, int& size)
{
	return m_net.sendData(data, size, m_serverIP, m_port);
}

int Client::send(char* data)
{
	int size = strlen(data);
	return m_net.sendData(data, size, m_serverIP, m_port);
}

int Client::recv(void * data, int& size)
{
	m_net.readData(data, size, m_serverIP, m_port);
	if (size > 0)
		return NET_RESPONSE;

	return NET_NORESPONSE;
}

int Client::recv(void * data)
{
	int size = 256;
	m_net.readData(data, size, m_serverIP, m_port);
	if (size > 0)
		return NET_RESPONSE;

	return NET_NORESPONSE;
}
