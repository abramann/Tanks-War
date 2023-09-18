// object.cpp
// Author: abramann

#include "object.h"
#include "game.h"
#include "map.h"
#include "timer.h"

Object::Object()
{
}

Object::~Object()
{
	if (m_pMap)
		m_pMap->removeObject(this);
}

void Object::initialize(std::string texture, const Game * pGame)
{
	m_pGame = pGame;
	m_pAudio = pGame->getAudio();
	m_pMap = pGame->getMap();
	m_pMap->addObject(this);
	Image::initialize(texture, pGame);
}

void Object::update()
{
	Image::update();
}

void Object::executeBack()
{
	if (m_playSound)
		playSoundBack();
}

void Object::executeDie()
{
	if (m_playSound)
		playSoundDie();
}

void Object::executeForward()
{
	if (m_playSound)
		playSoundForward();
}

void Object::executeLeft()
{
	if (m_playSound)
		playSoundLeft();
}

void Object::executeRight()
{
	if (m_playSound)
		playSoundRight();
}

void Object::damage(float dmg)
{
	m_health -= dmg;
#ifdef _SERVER_BUILD
	if (m_health <= 0)
		executeDie();
#endif
}