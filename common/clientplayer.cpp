#include "clientplayer.h"
#include "game.h"
#include "fileio.h"
#include "camera.h"

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
	TextureManger* pTextureManger = game->getTextureManger();
	Texture* pTexture = pTextureManger->getTexture(TEXTURE_PLAYER_TANK);
	std::string name = FileIO::readClientInfo().name;
	Player::initialize(id, name.c_str(), pTexture, game);
	m_pCamera = m_pGraphics->getCamera();
}

void ClientPlayer::update(float frameTime)
{
	m_act = PLAYER_ACT_NONE;
	if (m_handleInput)
		handleInput();

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
	m_act = PLAYER_ACT_RIGHT;
	Object2::executeRight();
}

void ClientPlayer::executeLeft()
{
	m_act = PLAYER_ACT_LEFT;
	Object2::executeLeft();
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
	else if (m_pInput->isKeyIn(m_right))
		executeRight();
	else if (m_pInput->isKeyIn(m_left))
		executeLeft();
}
