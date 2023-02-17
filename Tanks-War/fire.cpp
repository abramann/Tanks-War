#include "fire.h"

Fire::Fire() : m_active(false)
{
	m_release = false;
	m_bomb = false;
}

Fire::~Fire()
{
}

void Fire::initialize(FireData fireData, TextureManger* explostion, Map* map, TextureManger* textureManger, Graphics* graphics)
{
	m_fireData = fireData;
	m_pMap = map;
	m_pExplostion = explostion;
	m_pFire = textureManger;
	Image::initialize(0, 0, 1, 1, false, 20, textureManger, graphics);
}

void Fire::update(float frameTime)
{
	m_spriteData;
	while (m_release)
	{
		if (m_pMap->outOfRange(*this))
		{
			setDefaultMode();
			break;
		}

		Object* object = m_pMap->collided(*this);
		if (object != nullptr)
		{
			object->damage(m_fireData.healthDecrease);
			m_release = false;
			m_bomb = true;
			setExplosionMode();
			break;
		}
		if (m_releaseType == RELEASE_TRACE)
			releaseTraceUpdate(frameTime);
		else if (m_releaseType == RELEASE_NORMAL)
			releaseNormalUpdate(frameTime);
		break;
	}

	Image::update(frameTime);
}

void Fire::draw()
{
	if(m_release || m_bomb)
		Image::draw();
}

void Fire::release(Object* object, uint8_t releaseType)
{
	m_active = true;
	m_release = true;
	m_releaseType = releaseType;
	m_spriteData;
	if (m_releaseType == RELEASE_NORMAL)
	{
		m_objectData.x = object->getX();
		m_objectData.y = object->getY();
		m_objectData.angle = object->getAngle();
	}
	else if (m_releaseType = RELEASE_TRACE)
	{
		m_pTarget = object;
	}

	V2 site = getObjectFocuusPoint(*object, *this);
	setXY(site).setAngle(object->getAngle());
}

void Fire::setExplosionMode()
{
	setTexture(m_pExplostion);
	setDefaultTextureInfo();
	setDefaultColumnRow();
	setAnimate(true);
}

void Fire::endFrame()
{
	setDefaultMode();
}

void Fire::setDefaultMode()
{
	m_release = false;
	m_bomb = false;
	m_active = false;
	setTexture(m_pFire);
	setDefaultTextureInfo();
	setDefaultColumnRow();
	setAnimate(false);
	setX(0).setY(0);
}

void Fire::releaseTraceUpdate(float frameTime)
{
}

void Fire::releaseNormalUpdate(float frameTime)
{
	float sinA = sin(m_objectData.angle);
	sinA = _round(sinA);
	float cosA = cos(m_objectData.angle);
	cosA = _round(cosA);
	xAdd(m_fireData.speed*(sinA));
	yDec(m_fireData.speed*(cosA));
}
