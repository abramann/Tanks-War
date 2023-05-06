#include "object2.h"
#include "game.h"

Object2::Object2()
{
}

Object2::~Object2()
{
}

void Object2::initialize(Texture * texture, const Game* game)
{
	m_pGame = game;
	m_pAudio = game->getAudio();
	m_pMap = game->getMap();
	Image2::initialize(texture, game);
}

void Object2::update(float frameTime)
{
	Image2::update(frameTime);
}

void Object2::executeBack()
{
	if (m_playSound)
		playSoundBack();
}

void Object2::executeDie()
{
	if (m_playSound)
		playSoundDie();
}

void Object2::executeForward()
{
	if (m_playSound)
		playSoundForward();
}

void Object2::executeLeft()
{
	if (m_playSound)
		playSoundLeft();
}

void Object2::executeRight()
{
	if (m_playSound)
		playSoundRight();
}

void Object2::damage(float dmg)
{
	m_health -= dmg;
	if (m_health <= 0)
		executeDie();
}



