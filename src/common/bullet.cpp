// bullet.cpp
// Author: abramann

#include "bullet.h"
#include "game.h"
#include "tank.h"
#include "texturemanger.h"
#include "texture.h"
#include "map.h"
#include "timer.h"

Bullet::Bullet(const Game * pGame, Tank * pTank) : m_pTank(pTank), m_hit(false), m_finish(false)
{
	m_pGame = pGame;
	m_pMap = m_pGame->getMap();
	m_pAudio = m_pGame->getAudio();
	Image::initialize("bullet", m_pGame);
	executeLaunch();
}

Bullet::~Bullet()
{
}

void Bullet::update()
{
	Image::update();
	if (m_finish)
		return;
	if (!m_hit)
	{
		executeLaunching();
		if (m_pMap->isCollided(this))
			executeHit();
	}
}

V3 Bullet::getBulletLaunchPosition()
{
	Space s = m_pTank->getSpace();
	V3 position;
	position.x = (s.v4.x + (s.v3.x - s.v4.x) / 2);
	position.y = (s.v4.y + (s.v3.y - s.v4.y) / 2);
	return position;
}

void Bullet::executeLaunch()
{
	m_speed = m_pTank->getBulletSpeed();
	m_damage = m_pTank->getBulletDamage();
	m_rotate = m_pTank->getRotate();
	V3 pos = m_pTank->getPosition();
	float f1 = 1.0f - abs((0.636619772f *m_rotate.z));
	m_position = getBulletLaunchPosition();
	executeLaunching();
}

void Bullet::executeHit()
{
	m_hit = true;
	Object* pObject = m_pMap->getObject(getSpace());
	if (pObject)
	{
		m_pTank->addInflictedDamage(m_damage);
		pObject->damage(m_damage);
	}
	Image::initialize("bullet-destroy", m_pGame, textureNS::TEXTURE_BULLET_ROWS_COLUMNS, textureNS::TEXTURE_BULLET_ROWS_COLUMNS, logicNS::UPDATE_DELAY_BULLET);
	m_pAudio->play("bullet-explosion");
}

void Bullet::executeAnimateRepeat()
{
	m_finish = true;
}

void Bullet::executeLaunching()
{
	auto timeFactor = m_pTimer->getTimeFactor();
	float incX = m_speed*sin(m_rotate.z)*timeFactor,
		incY = m_speed*cos(m_rotate.z)*timeFactor;
	m_position;
	addX(-incX);
	addY(incY);
}