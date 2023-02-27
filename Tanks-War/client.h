#pragma once
#include "always.h"
#include <net.h>

class Client
{
public:

	Client();
	~Client();
	int initialize(char* serverIP);
	template <typename T>
	int send(void* data);
	int send(void* data, int& size);
	int send(char* data);
	template <typename T>
	int recv(void* data);
	template<typename T>
	int recv(void * data, bool wait);
	int recv(void* data, int& size);
	int recv(void* data);

private:

	Net m_net;
	char m_serverIP[10];
	unsigned short m_port;
	uint8_t m_protocol;
};

template<typename T>
inline int Client::send(void * data)
{
	int size = sizeof(T);
	return send(data, size);
}

template<typename T>
inline int Client::recv(void * data)
{
	int size = sizeof(T);
	return recv(data, size);
}

template<typename T>
inline int Client::recv(void * data, bool wait)
{
	int result;
	do
	{
		int size = sizeof(T);
		result = recv(data, size);
		if (size > 0)
			break;

		Sleep(200);
	} while (wait);

	return result;
}
