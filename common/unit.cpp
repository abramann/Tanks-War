#include "unit.h"

Unit::Unit()
{
}


Unit::~Unit()
{
}

void Unit::initialize(
	uint16_t width, uint16_t height, uint8_t columns, uint8_t rows, bool animate,
	float updateDelay, float health, float speed, Map* map, Texture* death,
	Texture* texture, Audio* audio, Graphics* graphics)
{
	m_pMap = map;
	Object::initialize(width, height, columns, rows, animate,
		updateDelay, health, speed, death, texture,
		graphics);
}

void Unit::initialize(Map * map, TextureManger* textureManger, Texture * texture, Audio* audio, Graphics * graphics)
{
	m_pMap = map;
	Object::initialize(texture, textureManger, audio, graphics);
}

void Unit::executeForward(float frameTime)
{
	float y = getY() - (m_objectInfo.speed);
	y = m_pMap->passY(getX() + getWidth() / 2, y, getY(), getHeight());
	setY(y);
}

void Unit::executeBack(float frameTime)
{
	float y = getY() + (m_objectInfo.speed) + getHeight();
	y = m_pMap->passY(getX(), y, getY(), getHeight()) - getHeight();
	setY(y);
}

void Unit::executeRight(float frameTime)
{
	float x = getX() + (m_objectInfo.speed) + m_spriteData.width;
	x = m_pMap->passX(x, m_spriteData.x, getY(), getWidth()) - getWidth();
	setX(x);
}

void Unit::executeLeft(float frameTime)
{
	float x = getX() - (m_objectInfo.speed);
	x = m_pMap->passX(x, getX(), getY(), getWidth());
	setX(x);
}