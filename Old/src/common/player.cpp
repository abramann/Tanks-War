// player.cpp
// Author: abramann

#include "player.h"
#include "game.h"
#include "texturemanger.h"
#include "map.h"

Player::Player() : m_id(0)
{
	memset(m_name, 0, gameNS::MAX_NAME_LEN);
}

Player::~Player()
{
}

void Player::initialize(PlayerID id, const std::string& name, PlayerType playerType, const Game * pGame)
{
	m_id = id;
	strcpy(m_name, name.c_str());
	std::string tex = (playerType == PLAYER_SELF) ? "player-tank" : "enemy-tank";
	Tank::initialize(tex, pGame);
	m_pMap->addObject(this);
}

void Player::damage(float dmg)
{
	m_health -= dmg;
#ifdef _CLIENT_BUILD
	if (!m_pGame->isOnline() && m_health <= 0)
		executeDie();
#else
	if (m_health <= 0)
		executeDie();
#endif
}

void Player::executeDie()
{
	m_pMap->removeObject(this);
	Tank::executeDie();
}

void Player::executeAnimateRepeat()
{
	m_pMap->addObject(this);
	Tank::executeAnimateRepeat();
#ifdef _CLIENT_BUILD
	if (!m_pGame->isOnline())
	{
		//m_health = 100;
		//setPosition(m_pMap->getRandomEmptySpace(this).v1);
		reset();
		setPosition(m_pMap->getRandomEmptySpace(this).v1);
	}
#endif
}

void Player::reset()
{
	m_rotate = V3();
	m_position = V3();
	m_health = logicNS::HEALTH_TANK;
	m_velocity = logicNS::VELOCITY_TANK;
	m_inflictedDamage = 0;
	m_madeKills = 0;
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