// object.cpp
// Author: abramann

#include "object.h"
#include "game.h"
#include "map.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::initialize(Texture * texture, const Game* game)
{
	m_pGame = game;
	m_pAudio = game->getAudio();
	m_pMap = game->getMap();
	m_pMap->addObject(this);
	Image::initialize(texture, game);
}

void Object::update(float frameTime)
{
	Image::update(frameTime);
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
	if (m_health <= 0)
		executeDie();
}



