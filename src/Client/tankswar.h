// tankswar.h
// Author: abramann

#pragma once

#include "..\common\game.h"
#include "..\common\tank.h"
#include "..\common\map.h"
#include "..\common\inlined.inl"
#include "client.h"

class ServerPlayer;
class AIPlayer;

typedef ServerPlayer RemoteClient;

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	virtual void initialize(HINSTANCE hInstance, HWND hwnd);
	virtual void update();
	virtual void render();
	virtual void communicate();
	virtual bool isOnline() const { return (m_status == clientNS::CLIENT_CONNECTED); }
	virtual void renderScene();
	virtual void updateScene();

	bool connect();
	void disconnect();
	char* getServerIP() { return m_clientInfo.serverIP; }
	char* getClientName() { return m_clientInfo.name; }
	std::shared_ptr<RemoteClient> findRemoteClientByID(PlayerID id);
	clientNS::ClientStatus getStatus() const { return m_status; }
	void setServerPort(Port port);
	Port getServerPort() const { return m_clientInfo.serverPort; }
	ThisClient* getThisClient() { return &m_thisClient; }
	ThisPlayer* getThisPlayer() { return &m_thisPlayer; }
	void updateClientInfo();
	bool applyReceivedGameProperties();
	void dispatchPlayerAct();
	void executeClientAct();
	int32 getExistClients() const { return m_pRemoteClient.size() + 1; }
	std::string getGameMap() const { return m_map; }
	void setGameMap(const std::string& gameMap) { debuggerBreak(isOnline(), "Invalid call!/n"); strcpy(m_map, gameMap.c_str()); }
	bool onStartSoloPlayerGame();
	bool isSoloGameStarted() const { return m_soloGameStarted; }
	void quitSoloGame();

private:

	void heartbeat();
	void applyUpdateClientGameStatus();
	void applyDisconnect();
	void applyClientGameAttribute();

	clientNS::ClientStatus m_status;
	PlayerID m_id;
	char m_map[gameNS::MAX_NAME_LEN];
	const char* m_pSData, *m_pRData;
	int16 m_clients;
	CpsDisconnect* m_pCpsDisconnect;
	CpsHeartbeat* m_pCpsHeartbeat;
	CpsJoin* m_pCpsJoin;
	SpsJoin* m_pSpsJoin;
	SpsClientGameStatus* m_pSpsClientGameStatus;
	SpsClientInitialData* m_pSpsClientInitialData;
	SpsDisconnect* m_pSpsDisconnect;
	SpsPlayerAct* m_pSpsPlayerAct;
	SpsClientGameAttribute* m_pSpsClientGameAttribute;
	CpsPlayerAct* m_pCpsPlayerAct;
	PacketType* m_pPacketType;
	ClientInfo m_clientInfo;
	std::vector<std::shared_ptr<RemoteClient>> m_pRemoteClient;
	ThisClient m_thisClient;
	ThisPlayer m_thisPlayer;
	std::vector<std::shared_ptr<AIPlayer>> m_pAIPlayer;
	bool m_soloGameStarted;
};