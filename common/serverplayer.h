// serverplayer.h
// Author: abramann

#ifndef _SERVERPLAYER_H
#define _SERVERPLAYER_H
#include "player.h"

#ifdef _SERVER_BUILD
class Server;
#endif

class ServerPlayer : public Player
{
public:

	ServerPlayer();
	~ServerPlayer();
	virtual void initialize(PlayerID id, const char* name, const Game* game);
#ifdef _SERVER_BUILD
	virtual void damage(float dmg);
	
private:

	Server* m_pServer;
#endif

};


#endif