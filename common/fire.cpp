#include "fire.h"
#include "fileio.h"

Fire::Fire() : m_active(false)
{
	m_release = false;
	m_bomb = false;
}

Fire::~Fire()
{
	
}

void Fire::initialize(Object* object, Map* map, Texture* explosion, Texture* texture, Graphics* graphics)
{
	m_pObject = object;
	m_pMap = map;
	m_pExplostion = explosion;
	m_pFire = texture;
//	Image::initialize(texture,, graphics);
}

void Fire::initialize(Object* object, Map* map, TextureManger* textureManger, Texture* texture, Graphics* graphics)
{
	Image::initialize(texture, textureManger, graphics);
	m_pObject = object;
	m_pMap = map;
	m_pFire = texture;
	m_fireInfo = *textureManger->getTextureInfo(m_pFire->getNumber())->fireInfo;
	m_pExplostion = textureManger->getTexture(m_fireInfo.endTexture);
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

		Object* object = m_pMap->isCollided(*this);
		if (object != nullptr && object != m_pObject)
		{
			m_release = false;
			m_bomb = true;
			object->damage(m_fireInfo.damage);
			setExplosionMode();
			break;
		}

		if (m_releaseType == RELEASE_TRACE)
			releaseTraceUpdate(frameTime);
		else if (m_releaseType == RELEASE_NORMAL)
			releaseNormalUpdate(frameTime);

		break;
	}

	m_spriteData;
	Image::update(frameTime);
}

void Fire::draw()
{
		Image::draw();
}

void Fire::release(uint8_t releaseType)
{
	m_active = true;
	m_release = true;
	m_releaseType = releaseType;
	if (m_releaseType == RELEASE_NORMAL)
	{
		m_objectData.x = m_pObject->getX();
		m_objectData.y = m_pObject->getY();
		m_objectData.angle = m_pObject->getAngle();
	}
	else if (m_releaseType = RELEASE_TRACE)
	{
	}

	V2 site = getObjectFocuusPoint(*m_pObject, *this);
	site.y -= m_pObject->getWidth() * cos(m_pObject->getAngle());
	site.x += m_pObject->getHeight() * sin(m_pObject->getAngle());
	setXY(site);
	setAngle(m_pObject->getAngle());
}

void Fire::setExplosionMode()
{
	setTexture(m_pExplostion);
	setDefaultImageInfo();
	setDefaultColumnRow();
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
	setDefaultImageInfo();
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
	xAdd(m_fireInfo.speed*(sinA));
	yAdd(m_fireInfo.speed*(cosA));
}
