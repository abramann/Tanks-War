#pragma once
#include "always.h"
#include <net.h>

class Server
{
public:
	
	Server();
	~Server();
	int initialize();
	void getClients();	
	template <typename T>
	int send(void* data, uint8_t id);
	template <typename T>
	int send(void* data);
	template <typename T>
	int8_t recv(void* data);

private:

	Net m_net;
	unsigned short m_playerPort[3];
	unsigned short m_port;
	uint8_t m_protocol;
	char m_playerIP[40];
	char m_data[256];
};


template<typename T>
inline int Server::send(void * data, uint8_t id)
{
	int size = sizeof(T);
	int r = m_net.sendData(data, size, (const char*)m_playerIP, m_playerPort[id]);
	return r;
}

template<typename T>
inline int Server::send(void * data)
{
	int result = send<T>(data, 0);
	if (result != netNS::NET_OK)
		return result;

	return send<T>(data, 1);
}

template<typename T>
inline int8_t Server::recv(void * data)
{
	const USHORT pt0 = m_playerPort[0];
	const USHORT pt1 = m_playerPort[1];
	unsigned short port;
	int size = sizeof(T);
	m_net.readData(data, size, m_playerIP , port);
	if (size > 0)
		for (int i = 0; i < 2; i++)
		{
			m_playerPort[0] = pt0;
			m_playerPort[1] = pt1;
			if (port == m_playerPort[i])
				return i;
		}
	return NET_NORESPONSE;
}
