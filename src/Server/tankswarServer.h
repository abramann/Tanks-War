// tankswarserver.h
// Author: abramann

#pragma once

#include "..\common\game.h"
#include "server.h"
#include "..\common\serverplayer.h"
//class ServerPlayer;

typedef ServerPlayer Client;

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
	void disconnectClient(std::shared_ptr<Client> pClient);
	void disonnectInactiveClient();
	void serverStart();
	void serverShutdown();
	void applyPlayerMove();
	int32 getMaxClients() const { return m_maxClients; }
	Port* getPort() { return &m_port; }
	void post(int size = networkNS::MAX_PACKET_SIZE);
	void send(Client* pClient, int size = networkNS::MAX_PACKET_SIZE);
	void resetClientGameState(Client* pClient);
	void updateClientGameState(Client* pClient);
	bool isActive() const { return m_active; }
	void setPort(Port port);
	void setMaxClients(int32 maxClients);
	void setMap(std::string map) { m_map = map; }
	void getIP(char* pIP) { m_pServer->getIP(pIP); }
	std::string getMap() const { return m_map; }
	ServerStatus getStatus() const { return m_status; }

private:

	PlayerID generateClientID() const;
	std::string getRandomMap() const;
	bool recv();

	bool m_active;
	Port m_port, * m_pReceiverPort;
	std::shared_ptr<Client> m_pReceiverClient;
	int32 m_maxClients;
	ServerStatus m_status;
	std::vector<std::shared_ptr<ServerPlayer> > m_pClient;
	const char* m_pSData, * m_pRData, * m_pReceiverIP;
	std::string m_map;
	CpsJoin* m_pCpsJoin;
	CpsHeartbeat* m_pCpsHeartbeat;
	CpsDisconnect* m_pCpsDisconnect;
	SpsJoin* m_pSpsJoin;
	SpsClientGameState* m_pSpsClientGameState;
	SpsDisconnect* m_pSpsDisconnect;
	PacketType* m_pPacketType;
};