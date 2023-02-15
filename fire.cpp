#include "fire.h"

Fire::Fire()
{
}

Fire::~Fire()
{
}

void Fire::initialize(FireData fireData, Map* map, TextureManger* textureManger, Graphics* graphics)
{
	m_fireData = fireData;
	Image::initialize(0, 0, 1, 1, false, 20, textureManger, graphics);
}

void Fire::update(float frameTime)
{
	if (m_release)
	{
		m_spriteData;
		if (m_releaseType == RELEASE_TRACE)
			releaseTraceUpdate(frameTime);
		else if (m_releaseType == RELEASE_NORMAL)
			releaseNormalUpdate(frameTime);
	}

	Image::update(frameTime);
}

void Fire::draw()
{
	//if (m_release)
		Image::draw();
}

Fire & Fire::release(const Unit& object, uint8_t releaseType)
{
	m_release = true;
	m_releaseType = releaseType;
	if (m_releaseType == RELEASE_NORMAL)
	{
		m_objectData.x = object.getX();
		m_objectData.y = object.getY();
		m_objectData.angle = object.getAngle();
	}
	else if (m_releaseType = RELEASE_TRACE)
	{
		m_target = object;
	}

	V2 site = getObjectFocuusPoint(object, *this);
	setXY(site).setAngle(object.getAngle());
	return *this;
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
