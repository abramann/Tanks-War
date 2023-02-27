#include "player.h"

Player::Player()
{
	ZeroMemory(&m_toServer, sizeof(PlayerToServer));
}

Player::~Player()
{
}

void Player::initialize(Map* map, TextureManger* textureManger, Input* input, Graphics* graphics)
{
	Tank::initialize(map, textureManger, textureManger->getTexture(TEXTURES::PLAYER_TANK), graphics);
	Tank::inputInitialize(input, W_KEY, S_KEY, D_KEY, A_KEY, Q_KEY);
}

void Player::update(PlayerState playerState)
{
	m_event = false;
	RemotePlayer::update(playerState);
}

void Player::executeForward(float frameTime)
{
	m_event = true;
	m_toServer.forward = true;
}

void Player::executeBack(float frameTime)
{
	m_event = true;
	m_toServer.back = true;
}

void Player::executeRight(float frameTime)
{
	m_event = true;
	m_toServer.right = true;
}

void Player::executeLeft(float frameTime)
{
	m_event = true;
	m_toServer.left = true;
}

void Player::executeAttack(float frameTime)
{
//	m_toServer. = true;
}
