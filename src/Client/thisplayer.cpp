// thisplayer.cpp
// Author: abramann

#include "thisplayer.h"
#include "input.h"
#include "camera.h"
#include "fileio.h"
#include "game.h"

ThisPlayer::ThisPlayer() : m_handleInput(true)
{
	m_forward = inputNS::W_KEY,
		m_back = inputNS::S_KEY,
		m_right = inputNS::D_KEY,
		m_left = inputNS::A_KEY,
		m_attack = inputNS::SPACE_KEY;
}

ThisPlayer::~ThisPlayer()
{
}

void ThisPlayer::initialize(const Game * pGame)
{
	m_pInput = pGame->getInput();
	Player::initialize(0, "Player0", PLAYER_SELF, pGame);
	m_pCamera = m_pGraphics->getCamera();
}

void ThisPlayer::update()
{
	if (m_handleInput)
		handleInput();

	m_pCamera->update(V3(m_position.x + getWidth() / 2, m_position.y + getHeight() / 2, 0));
	Player::update();
}

void ThisPlayer::executeDie()
{
	m_handleInput = false;
	Player::executeDie();
}

void ThisPlayer::handleInput()
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

void ThisPlayer::executeAnimateRepeat()
{
	m_handleInput = true;
	m_health = 100;
	Player::executeAnimateRepeat();
}