// serverplayer.cpp
// Author: abramann

#include "serverplayer.h"
#include "game.h"
#ifdef _SERVER_BUILD
#include "..\Server\tankswarServer.h"
#endif

ServerPlayer::ServerPlayer()
{
}

ServerPlayer::~ServerPlayer()
{
}

#ifdef _SERVER_BUILD
ServerPlayer::ServerPlayer(PlayerID id, const char* name, const char* ip, Port port,TanksWarServer* pTKServer)
{
	initialize(id, name, ip, port, pTKServer);
}

void ServerPlayer::initialize(PlayerID id, const char* name, const char* ip, Port port, TanksWarServer* pTKServer)
{
	m_pTKServer = pTKServer;
	strcpy(m_ip, ip);
	Player::initialize(id, name, PLAYER_ENEMY, pTKServer);
}

void ServerPlayer::damage(float dmg)
{
	Player::damage(dmg);
	m_pTKServer->updateClientGameState(this);
}

#else
void ServerPlayer::initialize(PlayerID id, const char* name, const Game * game)
{
	Player::initialize(id, name, PLAYER_ENEMY, game);
}
#endif