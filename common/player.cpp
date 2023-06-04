// player.cpp
// Author: abramann

#include "player.h"
#include "game.h"
#include "texturemanger.h"

Player::Player() : m_id(0)
{
	memset(m_name, 0, gameNS::MAX_NAME_LEN);
}

Player::~Player()
{
}

void Player::initialize(PlayerID id, const char* name, PlayerType playerType, const Game * game)
{
	m_id = id;
	strcpy_s(m_name, name);
	Texture* texture;
	TextureManger* pTextureManger = game->getTextureManger();
	if (playerType == PLAYER_SELF)
		texture = pTextureManger->getTexture(TEXTURE_PLAYER_TANK);
	else if (playerType == PLAYER_ENEMY)
		texture = pTextureManger->getTexture(TEXTURE_ENEMY_TANK);

	Tank::initialize(texture, game);
}

void Player::applyPlayerUpdate(const PlayerUpdate playerUpdate)
{
	m_position = playerUpdate.position;
	m_rotate = playerUpdate.rotate;
	m_health = playerUpdate.health;
}

PlayerUpdate Player::getPlayerUpdate() const
{
	PlayerUpdate pu;
	pu.position = m_position;
	pu.rotate = m_rotate;
	pu.health = m_health;
	pu.id = m_id;
	return pu;
}