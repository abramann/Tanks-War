// thisclient.cpp
// Author: abramann

#ifdef _CLIENT_BUILD
#include "..\common\fileio.h"
#include "..\common\camera.h"
#include "..\common\texturemanger.h"
#include "..\common\texture.h"
#include "..\common\input.h"
#include "thisclient.h"
#include "tankswar.h"

ThisClient::ThisClient()
{
	
}

ThisClient::~ThisClient()
{
}

void ThisClient::initialize(PlayerID id, TanksWar * pTW)
{
	m_pTW = pTW;
	m_pInput = m_pTW->getInput();
	std::string name = FileIO::readClientInfo().name;
	Player::initialize(id, name.c_str(), PLAYER_SELF, pTW);
	m_pCamera = m_pGraphics->getCamera();
}

void ThisClient::update()
{
	m_act = PLAYER_ACT_NONE;
	ThisPlayer::update();
	if (m_act != PLAYER_ACT_NONE)
		m_pTW->dispatchPlayerAct();

	
}

void ThisClient::executeForward()
{
	m_act = PLAYER_ACT_FORWRAD;
	Object::executeForward();
}

void ThisClient::executeBack()
{
	m_act = PLAYER_ACT_BACK;
	Object::executeBack();
}

void ThisClient::executeRight()
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

void ThisClient::executeLeft()
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

bool ThisClient::executeAttack()
{
	m_act |= PLAYER_ACT_ATTACK;
	return true;
}

void ThisClient::implementAttack()
{
	Player::executeAttack();
}


#endif