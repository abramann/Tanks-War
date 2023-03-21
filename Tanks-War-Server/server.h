#pragma once
#include "constants.h"
#include "net.h"

class Server
{
public:
	
	Server();
	~Server();
	void initialize();
	void start();
	void getClients();	
	void send(void* data, int size, PlayerID id);
	template <typename T>
	void send(void* data, PlayerID id);
	template <typename T>
	void send(void* data);
	void send(void* data, int size);
	void recvPlayerResponse();
	PlayerID recv(void* data, bool wait = false);
	const char* getIP() { m_net.getLocalIP(m_ip); return m_ip; }
	Port* getPort() { return &m_serverPort; }
	uint8_t getPlayers() const { return m_players; }
	uint8_t getConnectedPlayers() const { return m_clients; }
	void setPlayers(uint8_t players) { m_players = players; }
	ServerState getState() const { return m_state; }

private:

	void recv(void * data, char* ip, Port& port, bool wait = false);

	Net m_net;
	PlayerInfo* m_pPlayerInfo;
	Port* m_pPlayerPort;
	Port m_serverPort;
	char m_ip[netNS::IP_SIZE];
	Protocol m_protocol;
	uint8_t m_players;
	char* m_pPlayerIP;
	char m_map[MAX_NAME_LEN];
	uint8_t m_clients;
	ServerState m_state;

};


template<typename T>
inline void Server::send(void * data, PlayerID id)
{
	send(data, sizeof(T), id);
}

template<typename T>
inline void Server::send(void * data)
{
	int size = sizeof(T);
	send(data, size);
}

inline void Server::send(void * data, int size)
{
	for (int id = 0; id < m_players; id++)
		send(data, size, id);
}

inline void Server::recvPlayerResponse()
{
	char name[MAX_PLAYER_NAME] = { 0 };
	char ip[netNS::IP_SIZE] = { 0 };
	Port port;
	for (int i = 0; i < m_players; i++)
	{
		recv(name, ip, port, true);
		strcpy(&m_pPlayerIP[i*netNS::IP_SIZE], ip);
		strcpy(m_pPlayerInfo[i].name, name);
		m_pPlayerPort[i] = port;
		m_pPlayerInfo[i].id = i;
		send<decltype(m_players)>(&m_players, i);
	}

	send(m_pPlayerInfo, m_players*sizeof(PlayerInfo));
}

inline void Server::recv(void * data, char * ip, Port & port, bool wait)
{
	do
	{
		int size = MAX_PACKET_SIZE;
	    m_net.readData(data, size, ip, port);
		if (size > 0)
		{
			break;
		}
	} while (wait);

}
