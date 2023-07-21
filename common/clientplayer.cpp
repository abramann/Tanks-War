#ifdef _CLIENT_BUILD
#ifndef _CLIENT_H
#define _CLIENT_H
#include "clientplayer.h"
#include "game.h"
#include "fileio.h"
#include "camera.h"
#include "texturemanger.h"
#include "texture.h"

ClientPlayer::ClientPlayer() : m_handleInput(true)
{
}

ClientPlayer::~ClientPlayer()
{
}

void ClientPlayer::initialize(PlayerID id, Game * game)
{
	m_forward = W_KEY,
		m_back = S_KEY,
		m_right = D_KEY,
		m_left = A_KEY;
	m_pInput = game->getInput();
	std::string name = FileIO::readClientInfo().name;
	Player::initialize(id, name.c_str(), PLAYER_SELF, game);
	m_pCamera = m_pGraphics->getCamera();
}

void ClientPlayer::update(float frameTime)
{
	m_act = PLAYER_ACT_NONE;
	if (m_handleInput)
		handleInput();

	m_pCamera->update(m_position);
	Player::update(frameTime);
}

void ClientPlayer::executeForward()
{
	m_act = PLAYER_ACT_FORWRAD;
	Object2::executeForward();
}

void ClientPlayer::executeBack()
{
	m_act = PLAYER_ACT_BACK;
	Object2::executeBack();
}

void ClientPlayer::executeRight()
{
	Object2::executeRight();
	switch (m_act)
	{
	case PLAYER_ACT_FORWRAD:
		m_act = PLAYER_ACT_FORWARD_RIGHT;
		break;
	case PLAYER_ACT_BACK:
		m_act = PLAYER_ACT_BACK_RIGHT;
		break;
	default:
		m_act = PLAYER_ACT_RIGHT;
		break;
	}
}

void ClientPlayer::executeLeft()
{
	Object2::executeLeft();
	switch (m_act)
	{
	case PLAYER_ACT_FORWRAD:
		m_act = PLAYER_ACT_FORWARD_LEFT;
		break;
	case PLAYER_ACT_BACK:
		m_act = PLAYER_ACT_BACK_LEFT;
		break;
	default:
		m_act = PLAYER_ACT_LEFT;
		break;
	}
}

void ClientPlayer::executeAttack()
{
	m_act = PLAYER_ACT_ATTACK;
}

void ClientPlayer::handleInput()
{
	if (m_pInput->isKeyIn(m_forward))
		executeForward();
	else if (m_pInput->isKeyIn(m_back))
		executeBack();
	if (m_pInput->isKeyIn(m_right))
		executeRight();
	else if (m_pInput->isKeyIn(m_left))
		executeLeft();
}

#endif
#endif