// serverplayer.cpp
// Author: abramann

#include "serverplayer.h"
#include "game.h"
#include "map.h"
#ifdef _SERVER_BUILD
#include "..\Server\tankswarServer.h"
#else
#include "..\Client\tankswar.h"
#endif

ServerPlayer::~ServerPlayer()
{
}

#ifdef _SERVER_BUILD
ServerPlayer::ServerPlayer(PlayerID id, const char* name, const char* ip, Port port, TanksWarServer* pTWServer)
{
	initialize(id, name, ip, port, pTWServer);
}

void ServerPlayer::initialize(PlayerID id, const char* name, const char* ip, Port port, TanksWarServer* pTWServer)
{
	m_pTWServer = pTWServer;
	strcpy(m_ip, ip);
	m_port = port;
	Player::initialize(id, name, PLAYER_ENEMY, pTWServer);
}

void ServerPlayer::damage(float dmg)
{
	Player::damage(dmg);
}

void ServerPlayer::executeDie()
{
	Player::executeDie();
	m_pTWServer->executeServerPlayerDie(this);
	m_pMap->removeObject(this);
}

void ServerPlayer::executeAnimateRepeat()
{
	m_pTWServer->resetClientGameState(this);
	m_pTWServer->postClientGameState(this);
	Player::executeAnimateRepeat();
	m_pMap->addObject(this);
}

ClientGameState ServerPlayer::getClientGameState() const
{
	ClientGameState clientGameState;
	clientGameState.id = m_id;
	clientGameState.position = m_position;
	clientGameState.rotate = m_rotate;
	return clientGameState;
}

#else
ServerPlayer::ServerPlayer(PlayerID id, const char * name, TanksWar * pTW)
{
	initialize(id, name, pTW);
}

void ServerPlayer::initialize(PlayerID id, const char* name, TanksWar* pTW)
{
	m_pTW = pTW;
	Player::initialize(id, name, PLAYER_ENEMY, m_pTW);
}
#endif