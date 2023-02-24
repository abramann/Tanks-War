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
	int y = m_spriteData.y - (m_ObjectInfo.speed * frameTime);
	y = m_pMap->passY(m_spriteData.x + m_spriteData.width / 2, y, m_spriteData.y);
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[KEY_FORWARD]);
}

void Unit::executeBack(float frameTime)
{
	float y = m_spriteData.y + (m_ObjectInfo.speed * frameTime) + m_spriteData.height;
	y = m_pMap->passY(m_spriteData.x, y, m_spriteData.y) - m_spriteData.height;
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_BACK]);
}

void Unit::executeRight(float frameTime)
{
	float x = m_spriteData.x + (frameTime* m_ObjectInfo.speed) + m_spriteData.width;
	x = m_pMap->passX(x, m_spriteData.x, m_spriteData.y) - m_spriteData.width;
	setX(x);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_RIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	float newX = m_spriteData.x - (m_ObjectInfo.speed * frameTime);
	newX = m_pMap->passX(newX, m_spriteData.x, m_spriteData.y);
	setX(newX);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_LEFT]);
}