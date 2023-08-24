// server.h
// Author: abramann

#pragma once

#include "..\common\types.h"
#include "..\common\crc32.h"
#include "..\common\texturemanger.h"
#include "..\common\serverplayer.h"
#include <memory>

class TanksWarServer;
class Net;
class Timer;

class Server
{
public:

	friend class ServerPlayer;

	Server();
	~Server();
	void initialize(TanksWarServer* pTKServer);
	bool start();
	void close();
	void send(const std::string ip, Port port, int size = networkNS::MAX_PACKET_SIZE);
	bool recv();
	void getIP(char* ip) { m_net.getLocalIP(ip); }
	char* getReceiveBuffer() { return m_rData; }
	char* getSendBuffer() { return m_sData; }
	char* const getReceiverIP() { return m_rIP; }
	Port* const getReceiverPort() { return &m_rPort; }

private:

	void sbClear() { memset(m_sData, 0, networkNS::MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, networkNS::MAX_PACKET_SIZE); }

	TanksWarServer* m_pTKServer;
	Timer* m_pTimer;
	Net m_net;
	char m_rIP[netNS::IP_SIZE], m_rData[networkNS::MAX_PACKET_SIZE], m_sData[networkNS::MAX_PACKET_SIZE];
	Port m_rPort, * m_pServerPort;
};