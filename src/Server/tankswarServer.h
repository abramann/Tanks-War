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
	virtual void initialize(HINSTANCE hInstance, HWND hwnd);
	virtual void update();
	virtual void updateScene();
	virtual void render();
	virtual void communicate();
	virtual bool isOnline() const { return (m_status == serverNS::SERVER_RUNNING_HANDLING); }
	void handleNewClient();
	void createClient();
	bool clientExist();
	void applyClientHeartbeat();
	void applyClientPlayerAct();
	void disconnectClient(std::shared_ptr<Client> pClient);
	void disonnectInactiveClient();
	void serverStart();
	void serverShutdown();
	void applyPlayerMove();
	int32 getMaxClients() const { return m_maxClients; }
	size_t getConnectedClients() const { return m_pClient.size(); }
	Port* getPort() { return &m_port; }
	void post(int32 size = networkNS::MAX_PACKET_SIZE);
	template<typename T>
	void post();
	void send(Client* pClient, int32 size = networkNS::MAX_PACKET_SIZE, bool sbClear = true);
	void reply(int32 size);
	template<typename T>
	void reply();
	void replyClientsInitialData();
	void resetClientGameState(Client* pClient);
	void postClientGameState(Client* pClient);
	void setPort(Port port);
	void setMaxClients(int32 maxClients);
	void setMap(std::string map) { m_map = map; }
	void getIP(char* pIP) { m_pServer->getIP(pIP); }
	std::string getMap() const { return m_map; }
	serverNS::ServerStatus getStatus() const { return m_status; }
	void executeServerPlayerDie(Client* pClient);

private:

	PlayerID generateClientID() const;
	std::string getRandomMap() const;
	bool recv();

	Port m_port, * m_pReceiverPort;
	std::shared_ptr<Client> m_pReceiverClient;
	int32 m_maxClients;
	serverNS::ServerStatus m_status;
	std::vector<std::shared_ptr<ServerPlayer> > m_pClient;
	const char* m_pSData, * m_pRData, * m_pReceiverIP;
	std::string m_map;
	CpsJoin* m_pCpsJoin;
	CpsHeartbeat* m_pCpsHeartbeat;
	CpsDisconnect* m_pCpsDisconnect;
	CpsPlayerAct* m_pCpsPlayerAct;
	SpsJoin* m_pSpsJoin;
	SpsDisconnect* m_pSpsDisconnect;
	SpsClientGameState* m_pSpsClientGameState;
	SpsClientInitialData* m_pSpsClientInitialData;
	SpsPlayerAct* m_pSpsPlayerAct;
	PacketType* m_pPacketType;
};

template<typename T>
inline void TanksWarServer::post()
{
	post(sizeof(T));
}

template<typename T>
inline void TanksWarServer::reply()
{
	reply(sizeof(T));
}