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
	void postPlayersExist();
	void reply();
	void post();
	void clearClients();
	PlayerID recvID(bool wait = false);
	bool recv(bool wait = false);

	void getIP(char* ip) { m_net.getLocalIP(ip); }
	Port* getPort() { return &m_serverPort; }
	const uint8_t& getGamePlayers() const { return m_gamePlayers; }
	const uint8_t& getConnectedPlayers() const { return m_clientData.size(); }
	void setGamePlayers(const uint8_t& players) { m_gamePlayers = players; }
	const ServerState& getState() const { return m_state; }
	bool isStarted() const { return (m_state == SERVER_HANDLING || m_state == SERVER_WAITING) ? true : false; }
	void checkClients();
	
private:

	bool addClient();
	void removeLastClient();
	void removeClient(PlayerID id);
	void present();
	void sbClear() { memset(m_sData, 0, MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, MAX_PACKET_SIZE); }
	PlayerID generateID() { return _rand(255); }
	PlayerID getLastRecieverId();

	Map* m_pMap;
	Net m_net;
	Port m_serverPort;
	char m_IP[netNS::IP_SIZE], m_sIP[netNS::IP_SIZE], m_map[MAX_NAME_LEN], m_crc32[CRC32_SIZE],
		m_rData[MAX_PACKET_SIZE], m_sData[MAX_PACKET_SIZE];
	uint8_t m_gamePlayers;
	ServerState m_state;
	std::vector<ClientData> m_clientData;
	Port m_port;
//	PlayerID m_id;

	CpsIni* m_pCpsIni;
	CpsPresent* m_pCpsPresent;
	CpsDisconnect* m_pCpsDisconnect;
	SpsIni* m_pSpsIni;
	SpsPlayersExist* m_pSpsPlayersExist;
	SpsPlayersIniData* m_pSpsPlayerIniData;
	SpsDisconnect* m_pSpsDisconnect;

	PacketType* m_pPacketType;
};