#include "serverplayer.h"
#include "game.h"

ServerPlayer::ServerPlayer()
{
}


ServerPlayer::~ServerPlayer()
{
}

void ServerPlayer::initialize(PlayerID id, const char * name, Game * game)
{
	TextureManger* pTextureManger = game->getTextureManger();
	Texture* pTexture = pTextureManger->getTexture(TEXTURE_ENEMY_TANK);
	Player::initialize(id, name, pTexture, game);
}
