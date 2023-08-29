// tankswar.h
// Author: abramann

#pragma once

#include "..\common\game.h"
#include "..\common\tank.h"
#include "..\common\map.h"
#include "client.h"

class ServerPlayer;

typedef ServerPlayer RemoteClient;

class TanksWar : public Game
{
public:

	TanksWar();
	~TanksWar();
	void initialize(HINSTANCE hInstance, HWND hWnd);
	void update();
	void render();
	void communicate();
	bool connect();
	void disconnect();
	char* getServerIP() { return m_clientInfo.serverIP; }
	char* getClientName() { return m_clientInfo.name; }
	clientNS::ClientStatus getStatus() const { return m_status; }
	void setServerPort(Port port);
	Port getServerPort() const { return m_clientInfo.serverPort; }
	bool isConnected() { return false; }
	void updateClientInfo();
	void recvGameProperties();

private:

	clientNS::ClientStatus m_status;
	PlayerID m_id;
	char m_map[gameNS::MAX_NAME_LEN];
	const char* m_pSData, *m_pRData;
	int16 m_clients;
	CpsDisconnect* m_pCpsDisconnect;
	CpsHeartbeat* m_pCpsHeartbeat;
	CpsJoin* m_pCpsJoin;
	SpsJoin* m_pSpsJoin;
	PacketType* m_pPacketType;
	ClientInfo m_clientInfo;
	std::vector<std::shared_ptr<RemoteClient>> m_pRemoteClient;
};