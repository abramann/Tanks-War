// tankswarserver.h
// Author: abramann

#pragma once

#include "..\common\game.h"
#include "server.h"

struct Client
{
	Client(PlayerID id, std::string name, std::string _ip, Port _port, const Game* game)
	{
		port = _port;
		strcpy_s(ip, _ip.c_str());
		serverPlayer.initialize(id, name.c_str(), game);
	}

	void update() { serverPlayer.update(); }
	void draw() { serverPlayer.draw(); }

	char ip[netNS::IP_SIZE];
	Port port;
	int64 heartbeatTime;
	ServerPlayer serverPlayer;
};

class TanksWarServer : public Game
{
public:

	TanksWarServer();
	~TanksWarServer();
	void initialize(HINSTANCE hInstance, HWND hwnd);
	void update();
	void updateGameState();
	void render();
	void communicate();
	void handleNewClient();
	void createClient();
	bool clientExist();
	void receiveClientHeartbeat();
	void disconnectClient(Client* pClient);
	void disonnectInactiveClient();
	void serverStart();
	void serverShutdown();
	void applyPlayerMove();
	int32 getGameMaxPlayers() const { return m_gameMaxPlayers; }
	Port* getPort() { return &m_port; }
	void post(int size = networkNS::MAX_PACKET_SIZE);
	void send(Client* pClient, int size = networkNS::MAX_PACKET_SIZE);
	void resetClientGameState(Client* pClient);
	void updateClientGameState(Client* pClient);

private:

	PlayerID generateClientID() const;
	/*
	void reply(int size = networkNS::MAX_PACKET_SIZE);
	void replyPlayersExist();
	void replyPlayersIniData();
	void post(int size = networkNS::MAX_PACKET_SIZE);
	void postPlayersIniData();
	void postPlayerUpdate(PlayerID id);
	void postPlayersUpdate();
	void postNewPlayer();
	*/

	Port m_port, * m_pReceiverPort;
	int32 m_gameMaxPlayers;
	ServerStatus m_status;
	std::vector<std::shared_ptr<Client>> m_pClient;
	const char* m_pSData, * m_pRData;
	char* m_pReceiverIP;

	CpsJoin* m_pCpsJoin;
	CpsHeartbeat* m_pCpsHeartbeat;
	CpsDisconnect* m_pCpsDisconnect;
	SpsJoin* m_pSpsJoin;
	SpsClientGameState* m_pSpsClientGameState;
	SpsDisconnect* m_pSpsDisconnect;
	PacketType* m_pPacketType;
};