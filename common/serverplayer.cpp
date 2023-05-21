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
	Player::initialize(id, name, PLAYER_ENEMY, game);
}
