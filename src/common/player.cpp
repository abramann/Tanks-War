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

void Player::initialize(PlayerID id, const char* pName, PlayerType playerType, const Game * pGame)
{
	m_id = id;
	strcpy(m_name, pName);
	std::string tex = (playerType == PLAYER_SELF) ? "player-tank" : "enemy-tank";
	Tank::initialize(tex, pGame);
}

#ifdef _CLIENT_BUILD
void Player::setClientGameStatus(const ClientGameStatus& clientGameStatus)
{
	m_id = clientGameStatus.id;
	m_position = clientGameStatus.position;
	m_rotate = clientGameStatus.rotate;
}

void Player::setClientGameAttribute(const ClientGameAttribute& clientGameAttribute)
{
	m_health = clientGameAttribute.health;
	m_velocity = clientGameAttribute.velocity;
	m_bulletDamage = clientGameAttribute.bulletDamage;
	m_bulletSpeed = clientGameAttribute.bulletSpeed;
	m_inflictedDamage = clientGameAttribute.inflictedDamage;
}
#endif