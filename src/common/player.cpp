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

void Player::initialize(PlayerID id, const char* name, PlayerType playerType, const Game * pGame)
{
	m_id = id;
	strcpy_s(m_name, name);
	std::string tex;
	if (playerType == PLAYER_SELF)
		tex = "player-tank";
	else if (playerType == PLAYER_ENEMY)
		tex = "enemy-tank";

	Tank::initialize(tex, pGame);
}

void Player::applyPlayerUpdate(const PlayerUpdate playerUpdate)
{
	m_position = playerUpdate.position;
	m_rotate = playerUpdate.rotate;
	m_health = playerUpdate.health;
	if (m_health < 0)
		executeDie();
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

#ifdef _CLIENT_BUILD
void Player::setClientGameState(ClientGameState clientGameState)
{
	m_id = clientGameState.id;
	m_position = clientGameState.position;
	m_rotate = clientGameState.rotate;
}
#endif