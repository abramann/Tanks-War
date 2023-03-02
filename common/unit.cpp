#include "unit.h"



Unit::Unit()
{
}


Unit::~Unit()
{
}

void Unit::initialize(uint16_t width, uint16_t height, uint8_t columns, uint8_t rows, bool animate, float updateDelay, float health, float speed, Map* map, Texture* death, Texture* texture, Graphics* graphics)
{
	m_pMap = map;
	Object::initialize(width, height, columns, rows, animate, updateDelay, health, speed, death, texture, graphics);
}

void Unit::initialize(Map * map, TextureManger* textureManger, Texture * texture, Graphics * graphics)
{
	m_pMap = map;
	Object::initialize(texture, textureManger, graphics);
}

void Unit::executeForward(float frameTime)
{
	float y = getY() - (m_ObjectInfo.speed);
	y = m_pMap->passY(getX() + getWidth() / 2, y, getY(), getHeight());
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[KEY_FORWARD]);
}

void Unit::executeBack(float frameTime)
{
	float y = getY() + (m_ObjectInfo.speed) + getHeight();
	y = m_pMap->passY(getX(), y, getY(), getHeight()) - getHeight();
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_BACK]);
}

void Unit::executeRight(float frameTime)
{
	float x = getX() + (m_ObjectInfo.speed) + m_spriteData.width;
	x = m_pMap->passX(x, m_spriteData.x, getY(), getWidth()) - getWidth();
	setX(x);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_RIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	float x = getX() - (m_ObjectInfo.speed);
	x = m_pMap->passX(x, getX(), getY(), getWidth());
	setX(x);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_LEFT]);
}