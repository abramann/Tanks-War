// client.h
// Author: abramann

#pragma once
#include "..\common\types.h"
#include "clientplayer.h"
#include <memory>

class TanksWar;
class Net;

class Client
{
public:

	Client();
	~Client();
	bool connect(char* ip, Port port);
	void disconnect();
	void initialize(TanksWar* game);
	char* getReceiveBuffer() { return m_rData; }
	char* getSendBuffer() { return m_sData; }
	void sbClear() { memset(m_sData, 0, networkNS::MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, networkNS::MAX_PACKET_SIZE); }
	bool recv();
	bool recv(int64 waitTime);
	void send(int size = networkNS::MAX_PACKET_SIZE);
	template<typename T>
	void send();

	TanksWar* m_pTK;
	Net m_net;
	Port m_port;
	char m_rData[networkNS::MAX_PACKET_SIZE], m_sData[networkNS::MAX_PACKET_SIZE];
	char* m_pServerIP;
	Port m_serverPort;
};

template<typename T>
inline void Client::send()
{
	int size = sizeof(T);
	this->send(size);
}
