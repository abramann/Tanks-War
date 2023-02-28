#include "player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::initialize(Map* map, TextureManger* textureManger, Input* input, Graphics* graphics)
{
	Tank::initialize(map, textureManger, textureManger->getTexture(TEXTURES::PLAYER_TANK), graphics);
}

void Player::update(float frameTime, PlayerState p0State)
{
	setPlayerState(p0State);
	Tank::update(frameTime);
}

void Player::draw()
{
	Tank::draw();
}

void Player::executeForward(float frameTime)
{
	m_toServer.forward = true;
}

void Player::executeBack(float frameTime)
{
	m_toServer.back = true;
}

void Player::executeRight(float frameTime)
{
	m_toServer.right = true;
}

void Player::executeLeft(float frameTime)
{
	m_toServer.left = true;
}

void Player::executeAttack(float frameTime)
{
//	m_toServer.attack = true;
}

void Player::setPlayerState(PlayerState playerState)
{
	setX(playerState.x).setY(playerState.y).setAngle(playerState.angle);
	setHealth(playerState.health);
//	setFireX();
}
