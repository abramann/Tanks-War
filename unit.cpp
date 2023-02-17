#include "unit.h"



Unit::Unit()
{
}


Unit::~Unit()
{
}

void Unit::initialize(int width, int height, int columns, int rows, bool animate, float updateDelay, float health, float speed, TextureManger * death, Map * map, TextureManger * textureManger, Graphics * graphics)
{
	m_pMap = map;
	Object::initialize(width, height, columns, rows, animate, updateDelay, health, speed, death, textureManger, graphics);
}
void Unit::executeForward(float frameTime)
{
	int y = m_spriteData.y - (m_speed * frameTime);
	y = m_pMap->passY(m_spriteData.x + m_spriteData.width / 2, y, m_spriteData.y);
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[KEY_FORWARD]);
}

void Unit::executeBack(float frameTime)
{
	int y = m_spriteData.y + (m_speed * frameTime) + m_spriteData.height;
	y = m_pMap->passY(m_spriteData.x, y, m_spriteData.y) - m_spriteData.height;
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_BACK]);
}

void Unit::executeRight(float frameTime)
{
	int x = m_spriteData.x + (frameTime*m_speed) + m_spriteData.width;
	x = m_pMap->passX(x, m_spriteData.x, m_spriteData.y) - m_spriteData.width;
	setX(x);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_RIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	int newX = m_spriteData.x - (m_speed * frameTime);
	newX = m_pMap->passX(newX, m_spriteData.x, m_spriteData.y);
	setX(newX);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_LEFT]);
}

void Unit::executeDeath()
{
	m_handleInput = false;
	m_deathExecuted = true;
	auto currentX = m_spriteData.x;
	auto currentY = m_spriteData.y;
	m_spriteData = m_death.getSpriteData();
	m_spriteData.x = currentX;
	m_spriteData.y = currentY;

	m_animate = m_death.isAnimated();

	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_DEATH]);

}