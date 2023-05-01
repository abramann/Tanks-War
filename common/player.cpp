#include "player.h"
#include "game.h"

Player::Player() : m_id(0)
{
	memset(m_name, 0, MAX_NAME_LEN);
}

Player::~Player()
{
}

void Player::initialize(PlayerID id, const char* name, Texture* texture, const Game * game)
{
	m_id = id;
	strcpy(m_name, name);
	Tank2::initialize(texture, game);
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
