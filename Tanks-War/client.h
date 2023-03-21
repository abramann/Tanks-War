#pragma once
#include "constants.h"
#include "map.h"
#include "net.h"

class Client
{
public:

	Client();
	~Client();
	void initialize(Map* map);
	bool connect();
	template <typename T>
	void send(void* data);
	void send(void* data, int& size);
	void send(char* text);
	int recv(void* data);
	void recv(void * data, bool wait);
	char* getPlayerName() { return m_clientInfo.playerName; }
	char* getServerIP() { return m_clientInfo.serverIP; }
	Port* getServerPort() { return &m_clientInfo.serverPort; }
	const uint8_t getState() const { return m_state; }
	const uint8_t& getServerPlayers() const { return m_serverPlayers; }
	char* getServerMap() { return (char*)m_map; }
private:
	
	Map* m_pMap;
	Net m_net;
	Port m_port;
	Protocol m_protocol;
	ClientInfo m_clientInfo;
	uint8_t m_serverPlayers;
	ClientState m_state;
	char m_map[MAX_NAME_LEN];
	char m_mapHash[MD5_LEN];
};
#include <encdec.h>
template<typename T>
inline void Client::send(void * data)
{
	int size = sizeof(T);
	send(data, size);
}

