// serverplayer.cpp
// Author: abramann

#include "serverplayer.h"
#include "game.h"

ServerPlayer::ServerPlayer()
{
}

ServerPlayer::~ServerPlayer()
{
}

void ServerPlayer::initialize(PlayerID id, const char * name, const Game * game)
{
#ifdef _SERVER_BUILD
	m_pServer = game->getServer();
#endif
	Player::initialize(id, name, PLAYER_ENEMY, game);
}

#ifdef _SERVER_BUILD
void ServerPlayer::damage(float dmg)
{
	Player::damage(dmg);
	m_pServer->postPlayerUpdate(m_id);
}
#endif