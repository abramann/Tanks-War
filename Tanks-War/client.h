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
	void update();
	bool connect();
	void present();
	void disconnect();
	void wait();
	void send();
	bool recv();
	void recv(bool wait);
	void initPlayers();
	char* getPlayerName() { return m_clientInfo.name; }
	const char* getServerIP() { return m_clientInfo.serverIP; }
	Port* getServerPort() { return &m_clientInfo.serverPort; }
	const uint8_t getState() const { return m_state; }
	const uint8_t& getGamePlayers() const { return m_gamePlayers; }
	const uint8_t& getConnectedPlayers()const { return m_connectedPlayers; }
	char* getGameMap() { return (char*)m_map; }
	bool isConnected() const { return (m_state == CLIENT_CONNECTED_WAITING || m_state == CLIENT_CONNECTED_PLAYING) ? true:false; }

private:
	
	void sbClear() { memset(m_sData, 0, MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, MAX_PACKET_SIZE); }

	Map* m_pMap;
	Net m_net;
	Port m_port;
	ClientInfo m_clientInfo;
	uint8_t m_connectedPlayers, m_gamePlayers;
	ClientState m_state;
	
	char m_map[MAX_NAME_LEN];
	PlayerID m_id;

	CpsIni* m_pCpsIni;
	CpsDisconnect* m_pCpsDisconnect;
	SpsIni* m_pSpsIni;
	SpsPlayersExist* m_pSpsPlayersExist;
	SpsPlayersInitData* m_pSpsPlayerIniData;
	CpsSeasson* m_pCpsSeasson;
	CpsPresent* m_pCpsPresent;
	PacketType* m_pPacketType;

	DWORD m_presentTime;
	char m_rData[MAX_PACKET_SIZE], m_sData[MAX_PACKET_SIZE];
	std::vector<PlayerIniData> m_playerData;
};

