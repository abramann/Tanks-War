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
	m_pTWServer->postClientGameAttribute(this);
}

void ServerPlayer::executeDie()
{
	Player::executeDie();
	m_pTWServer->executeServerPlayerDie(this);
}

void ServerPlayer::executeAnimateRepeat()
{
	m_pTWServer->resetClientGameStatus(this);
	m_pTWServer->postClientGameStatus(this);
	//Tank::executeAnimateRepeat();
	Player::executeAnimateRepeat();
}

ClientGameStatus ServerPlayer::getClientGameStatus() const
{
	ClientGameStatus clientGameStatus;
	clientGameStatus.id = m_id;
	clientGameStatus.position = m_position;
	clientGameStatus.rotate = m_rotate;
	return clientGameStatus;
}

ClientGameAttribute ServerPlayer::getClientGameAttribute() const
{
	ClientGameAttribute cga;
	cga.id = m_id;
	cga.health = m_health;
	cga.velocity = m_velocity;
	cga.bulletDamage = m_bulletDamage;
	cga.bulletSpeed = m_bulletSpeed;
	cga.inflictedDamage = m_inflictedDamage;
	return cga;
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