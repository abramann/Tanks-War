#include "player.h"

Player::Player() : m_event(false)
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

void Player::update()
{
	m_event = false;
	memset(&m_toServer, 0, sizeof(PlayerToServer));
	Tank::update(0);
}

void Player::update(PlayerState playerState)
{
	m_event = false;
	RemotePlayer::update(playerState);
}

void Player::update(ServerToPlayer serverToPlayer)
{
	if (m_id == 0)
		setState(serverToPlayer.p0);
	else if (m_id == 1)
		setState(serverToPlayer.p1);

	Tank::update(0);
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
