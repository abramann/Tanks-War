#pragma once
#include "constants.h"
#include "net.h"
#include "map.h"
#include "crc32.h"


class Server
{
public:

	Server();
	~Server();
	void initialize(Map* map);
	//	void waitClient();
	void update();
	void stop();
	void start();
	void getClients();
	void send(PlayerID id);
	void reply();
	void post();
	
	PlayerID recvID(bool wait = false);
	bool recv(bool wait = false);

	const char* getIP() { m_net.getLocalIP(m_sIP); return m_sIP; }
	Port* getPort() { return &m_serverPort; }
	uint8_t getGamePlayers() const { return m_gamePlayers; }
	uint8_t getConnectedPlayers() const { return m_clients; }
	void setGamePlayers(uint8_t players) { m_gamePlayers = players; }
	ServerState getState() const { return m_state; }

private:

	bool addClient();
	void removeClient();
	void sbClear() { memset(m_sData, 0, MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, MAX_PACKET_SIZE); }
	PlayerID getLastRecieverId();

	Map* m_pMap;
	Net m_net;
	Port m_serverPort;
	char m_IP[netNS::IP_SIZE], m_sIP[netNS::IP_SIZE], m_map[MAX_NAME_LEN], m_crc32[CRC32_SIZE],
		m_rData[MAX_PACKET_SIZE], m_sData[MAX_PACKET_SIZE];
	uint8_t m_clients, m_gamePlayers;
	ServerState m_state;
	std::vector<ClientData> m_clientData;
	Port m_port;
	PlayerID m_id;

	CpsIni* m_pCpsIni;
	SpsIni* m_pSpsIni;
	SpsPlayersExist* m_pSpsPlayersExist;
	SpsPlayersIniData* m_pSpsPlayerIniData;
	SpsDisconnect* m_pSpsDisconnect;

	PacketType* m_pPacketType;
};