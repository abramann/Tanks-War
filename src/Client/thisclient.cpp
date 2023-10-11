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

ThisClient::ThisClient() : m_handleInput(true)
{
	m_forward = inputNS::W_KEY,
		m_back = inputNS::S_KEY,
		m_right = inputNS::D_KEY,
		m_left = inputNS::A_KEY,
		m_attack = inputNS::SPACE_KEY;
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
	if (m_handleInput)
		handleInput();

	if (m_act != PLAYER_ACT_NONE)
		m_pTW->dispatchPlayerAct();

	m_pCamera->update(m_position);
	Player::update();
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

void ThisClient::executeAnimateRepeat()
{
	m_handleInput = true;
	m_health = 100;
	Player::executeAnimateRepeat();
}

void ThisClient::implementAttack()
{
	Player::executeAttack();
}

void ThisClient::executeDie()
{
	m_handleInput = false;
	Player::executeDie();
}

void ThisClient::handleInput()
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