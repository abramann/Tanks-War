#pragma once
#include "constants.h"
#include "net.h"

class Client
{
public:

	Client();
	~Client();
	int initialize(char* serverIP, char* playerName, uint8_t& players);
	template <typename T>
	void send(void* data);
	void send(void* data, int& size);
	void send(char* text);
	int recv(void* data);
	void recv(void * data, bool wait);
	
private:
	
	Net m_net;
	char m_serverIP[10];
	Port m_port , m_serverPort;
	Protocol m_protocol;
};

template<typename T>
inline void Client::send(void * data)
{
	int size = sizeof(T);
	send(data, size);
}

