// serverplayer.h
// Author: abramann

#pragma once

#include "player.h"

#ifdef _SERVER_BUILD
class TanksWarServer;
#else
class TanksWar;
#endif

class ServerPlayer : public Player
{
public:

	ServerPlayer() {};
	~ServerPlayer();
#ifdef _SERVER_BUILD
	ServerPlayer(PlayerID id, const char* name, const char* ip, Port port, TanksWarServer* pTKServer);
	
	virtual void initialize(PlayerID id, const char* name, const char* ip, Port port, TanksWarServer* pTKServer);
	virtual void damage(float dmg);
	char* getIP() { return m_ip; }
	Port getPort() { return m_port; }
	int64 getHeartbeatTime() { return m_heartbeatTime; }
	void setHeartbeatTime(int64 heartbeatTime) { m_heartbeatTime = heartbeatTime; }

private:

	TanksWarServer* m_pTKServer;
	char m_ip[netNS::IP_SIZE];
	Port  m_port;
	int64 m_heartbeatTime;

#else
	ServerPlayer(PlayerID id, const char* name, TanksWar* pTK);
	
	virtual void initialize(PlayerID id, const char* name, TanksWar* pTK);

private:

	TanksWar* m_pTK;
#endif
};