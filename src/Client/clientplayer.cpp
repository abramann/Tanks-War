// clientplayer.cpp
// Author: abramann

#ifdef _CLIENT_BUILD
#include "..\common\fileio.h"
#include "..\common\camera.h"
#include "..\common\texturemanger.h"
#include "..\common\texture.h"
#include "..\common\input.h"
#include "clientplayer.h"
#include "tankswar.h"

ClientPlayer::ClientPlayer() : m_handleInput(true)
{
	m_forward = inputNS::W_KEY,
		m_back = inputNS::S_KEY,
		m_right = inputNS::D_KEY,
		m_left = inputNS::A_KEY,
		m_attack = inputNS::F_KEY;
}

ClientPlayer::~ClientPlayer()
{
}

void ClientPlayer::initialize(PlayerID id, TanksWar * pTW)
{
	m_pTW = pTW;
	m_pInput = m_pTW->getInput();
	std::string name = FileIO::readClientInfo().name;
	Player::initialize(id, name.c_str(), PLAYER_SELF, pTW);
	m_pCamera = m_pGraphics->getCamera();
}

void ClientPlayer::update()
{
	m_act = PLAYER_ACT_NONE;
	if (m_handleInput)
		handleInput();

	if (m_act != PLAYER_ACT_NONE)
		m_pTW->dispatchPlayerAct();

	m_pCamera->update(m_position);
	Player::update();
}

void ClientPlayer::executeForward()
{
	m_act = PLAYER_ACT_FORWRAD;
	Object::executeForward();
}

void ClientPlayer::executeBack()
{
	m_act = PLAYER_ACT_BACK;
	Object::executeBack();
}

void ClientPlayer::executeRight()
{
	Object::executeRight();
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
	Object::executeLeft();
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

bool ClientPlayer::executeAttack()
{
	m_act |= PLAYER_ACT_ATTACK;
	return true;
}

void ClientPlayer::executeAnimateRepeat()
{
	m_handleInput = true;
	Player::executeAnimateRepeat();
}

void ClientPlayer::implementAttack()
{
	Player::playSoundAttack();
	Player::executeAttack();
}

void ClientPlayer::executeDie()
{
	m_handleInput = false;
	Player::executeDie();
}

void ClientPlayer::handleInput()
{
	if (m_pInput->isKeyDown(m_forward))
		executeForward();
	else if (m_pInput->isKeyDown(m_back))
		executeBack();
	if (m_pInput->isKeyDown(m_right))
		executeRight();
	else if (m_pInput->isKeyDown(m_left))
		executeLeft();
	if (m_pInput->isKeyDown(m_attack))
		executeAttack();
}

#endif