#pragma once
#include "constants.h"

#include "crc32.h"
#include "texturemanger.h"
#include "serverplayer.h"
#include <map>

class Game;
class Net;
class Map2;
class TextureManger;
class Timer;

struct ClientData
{
	char ip[netNS::IP_SIZE], name[MAX_NAME_LEN];
	Port port;
	DWORD presentTime;
	ServerPlayer serverPlayer;
	PlayerID getID() const { return serverPlayer.getID(); }
	const char* getName() const { return serverPlayer.getName(); }
	void setID(PlayerID id) { serverPlayer.setID(id); }
};

class Server
{
public:

	Server();
	~Server();
	void initialize(const Game* game);
	void update();
	void stop();
	void start();
	void send(PlayerID id);
	void reply();
	void replyPlayersExist();
	void replayPlayersIniData();
	void post();
	void postPlayersExist();
	void postPlayersIniData();
	void postPlayerUpdate(PlayerID id);
	void postPlayersUpdate();
	void postNewPlayer();
	void clearClients();
	PlayerID recvID(bool wait = false);
	bool recv(bool wait = false);
	void getClients();
	std::vector<ClientData> getClientData() { return m_clientData; }
	void getIP(char* ip) { m_net.getLocalIP(ip); }
	Port* getPort() { return &m_serverPort; }
	const uint8_t& getGamePlayers() const { return m_gameMaxPlayers; }
	const uint8_t& getConnectedPlayers() const { return m_clientData.size(); }
	const std::vector<ClientData>* getClientsData() const { return &m_clientData; }

	void setGamePlayers(const uint8_t& players) { m_gameMaxPlayers = players; }
	const ServerState& getState() const { return m_state; }
	bool isStarted() const { return (m_state == SERVER_RUNNING_HANDLING || m_state == SERVER_RUNNING_WAITING || m_state == SERVER_RUNNING_PREPGAME); }
	void checkClients();
	ClientData* getIDClientData(PlayerID requiredID);

private:

	void applyPlayerAct();
	bool addClient();
	void removeLastClient();
	void removeClient(PlayerID id);
	void present();
	void sbClear() { memset(m_sData, 0, MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, MAX_PACKET_SIZE); }
	PlayerID generateID() { return _rand(255); }
	PlayerID getLastRecieverId();

	const Game* m_pGame;
	Map2* m_pMap;
	Graphics* m_pGraphics;
	TextureManger* m_pTextureManger;
	Audio* m_pAudio;
	Timer* m_pTimer;

	Net m_net;
	Port m_serverPort;
	char m_IP[netNS::IP_SIZE], m_sIP[netNS::IP_SIZE], m_map[MAX_NAME_LEN], m_crc32[CRC32_SIZE],
		m_rData[MAX_PACKET_SIZE], m_sData[MAX_PACKET_SIZE];
	uint8_t m_gameMaxPlayers;
	ServerState m_state;
	std::vector<ClientData> m_clientData;
	Port m_port;
	CpsIni* m_pCpsIni;
	CpsPresent* m_pCpsPresent;
	CpsDisconnect* m_pCpsDisconnect;
	CpsPlayerAct* m_pCpsPlayerAct;
	SpsIni* m_pSpsIni;
	SpsPlayersExist* m_pSpsPlayersExist;
	SpsPlayersInitData* m_pSpsPlayerInitData;
	SpsDisconnect* m_pSpsDisconnect;
	SpsPlayerUpdate* m_pSpsPlayerUpdate;
	PacketType* m_pPacketType;
};