#pragma once
#include "constants.h"
#include "tank2.h"

class Game;
class Map2;
class TextureManger;
class Input;
class Timer;
class Net;

struct ClientData
{
	char name[MAX_NAME_LEN];
	PlayerID id;
	Tank2 playerTank;
};

class Client
{
public:

	Client();
	~Client();
	bool connect();
	bool isConnected() const { return (m_state == CLIENT_CONNECTED_WAITING || m_state == CLIENT_CONNECTED_PLAYING) ? true : false; }
	char* getGameMap() { return (char*)m_map; }
	char* getPlayerName() { return m_clientInfo.name; }
	const char* getServerIP() { return m_clientInfo.serverIP; }
	const uint8_t getState() const { return m_state; }
	const uint8_t& getConnectedPlayers()const { return m_clientData.size(); }
	const uint8_t& getGamePlayers() const { return m_gamePlayers; }
	Port* getServerPort() { return &m_clientInfo.serverPort; }
	std::vector<ClientData> getClientData() { return m_clientData; }
	void disconnect();
	void initialize(Game* game);
	void present();
	void update();

private:

	bool recv();
	void addNewPlayer();
	void initPlayers();
	void playersUpdate();
	void playerUpdate();
	void rbClear() { memset(m_rData, 0, MAX_PACKET_SIZE); }
	void recv(bool wait);
	void removeClient(PlayerID id);
	void sbClear() { memset(m_sData, 0, MAX_PACKET_SIZE); }
	void send();

	TextureManger* m_pTextureManger;
	Audio* m_pAudio;
	Graphics* m_pGraphics;
	Input* m_pInput;
	Map2* m_pMap;
	Timer* m_pTimer;
	Game* m_pGame;

	Net m_net;
	Port m_port;
	ClientInfo m_clientInfo;
	uint8_t m_gamePlayers;
	ClientState m_state;

	char m_map[MAX_NAME_LEN];
	PlayerID m_id;

	CpsIni* m_pCpsIni;
	CpsDisconnect* m_pCpsDisconnect;
	CpsSeasson* m_pCpsSeasson;
	CpsPresent* m_pCpsPresent;
	CpsPlayerAct* m_pCpsPlayerAct;
	SpsIni* m_pSpsIni;
	SpsPlayersExist* m_pSpsPlayersExist;
	SpsPlayersInitData* m_pSpsPlayerIniData;
	SpsPlayerUpdate* m_pSpsPlayerUpdate;

	PacketType* m_pPacketType;

	float m_presentTime;
	char m_rData[MAX_PACKET_SIZE], m_sData[MAX_PACKET_SIZE];
	std::vector<ClientData> m_clientData;
};
