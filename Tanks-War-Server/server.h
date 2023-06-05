// server.h
// Author: abramann

#ifndef _SERVER_H
#define _SERVER_H
#include "constants.h"

#include "crc32.h"
#include "texturemanger.h"
#include "serverplayer.h"
#include <memory>

class Game;
class Net;
class Map2;
class TextureManger;
class Timer;

struct ClientData
{
	ClientData(PlayerID id, const char* name, const char* ip, Port port, const Game* game)
	{
		_port = port;
		strcpy_s(_ip, ip);
		serverPlayer.initialize(id, name, game);
	}

	PlayerID getID() const { return serverPlayer.getID(); }
	const char* getName() const { return serverPlayer.getName(); }
	const char* getIP() const { return _ip; }
	Port getPort() const { return _port; }
	void update() { serverPlayer.update(); }
	void draw() { serverPlayer.draw(); }
	
	char _ip[netNS::IP_SIZE];
	Port _port;
	DWORD presentTime;
	ServerPlayer serverPlayer;

};

class Server
{
public:

	friend class ServerPlayer;

	Server();
	~Server();
	void initialize(const Game* game);
	void update();
	void stop();
	void start();

	std::vector<std::shared_ptr<ClientData> > getClientData() { return m_pClientData; }
	void getIP(char* ip) { m_net.getLocalIP(ip); }
	Port* getPort() { return &m_serverPort; }
	const uint8_t& getGamePlayers() const { return m_gameMaxPlayers; }
	const size_t getConnectedPlayers() const { return m_pClientData.size(); }
	const std::vector<std::shared_ptr<ClientData> >* getClientsData()  const { return &m_pClientData; }

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
	void sbClear() { memset(m_sData, 0, networkNS::MAX_PACKET_SIZE); }
	void rbClear() { memset(m_rData, 0, networkNS::MAX_PACKET_SIZE); }
	void send(PlayerID id, int size = networkNS::MAX_PACKET_SIZE);
	void reply(int size = networkNS::MAX_PACKET_SIZE);
	void replyPlayersExist();
	void replyPlayersIniData();
	void post(int size = networkNS::MAX_PACKET_SIZE);
	void postPlayersExist();
	void postPlayersIniData();
	void postPlayerUpdate(PlayerID id);
	void postPlayersUpdate();
	void postNewPlayer();
	void clearClients();
	PlayerID recvID(bool wait = false);
	bool recv(bool wait = false);
	void getClients();
	PlayerID generateID() { return _rand(255); }
	PlayerID getLastRecieverId();

	const Game* m_pGame;
	Map* m_pMap;
	Graphics* m_pGraphics;
	TextureManger* m_pTextureManger;
	Audio* m_pAudio;
	Timer* m_pTimer;

	Net m_net;
	Port m_serverPort;
	char m_IP[netNS::IP_SIZE], m_sIP[netNS::IP_SIZE], m_map[networkNS::MAX_PACKET_SIZE], m_crc32[CRC32_SIZE],
		m_rData[networkNS::MAX_PACKET_SIZE], m_sData[networkNS::MAX_PACKET_SIZE];
	uint8_t m_gameMaxPlayers;
	ServerState m_state;
	std::vector<std::shared_ptr<ClientData> > m_pClientData;
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
	SpsPlayerAct* m_pSpsPlayerAct;
	PacketType* m_pPacketType;
};


#endif