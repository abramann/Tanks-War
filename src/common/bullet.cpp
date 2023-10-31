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
		if (m_pMap->isCollided(getSpace(), { m_pTank }))
			executeHit();
	}
}

V3 Bullet::getBulletLaunchPosition()
{
	auto tankSpace = m_pTank->getSpace();
	float angle = m_pTank->getNegativeRotate();
	V3 start, end;
	float piOverTwo = angle / (-PI / 2);
	if (piOverTwo > 3)
		start = tankSpace.v1, end = tankSpace.v4;
	else if (piOverTwo > 2)
		start = tankSpace.v2, end = tankSpace.v1;
	else if (piOverTwo > 1)
		start = tankSpace.v3, end = tankSpace.v2;
	else
		start = tankSpace.v3, end = tankSpace.v4;

	return V3(getMin<float>({ start.x, end.x }) + (abs(start.x - end.x) / 2),
		getMin<float>({ start.y, end.y }) + (abs(start.y - end.y) / 2));
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

V3 Bullet::getRotateCenter() const
{
	return V3();
}

void Bullet::executeHit()
{
	m_hit = true;
	Object* pObject = m_pMap->getObject(getSpace());
	if (pObject)
	{
		float inflectedDamage = m_damage;
		if (m_pTank == pObject)
			inflectedDamage *= -1;

		m_pTank->addInflictedDamage(inflectedDamage);
		pObject->damage(m_damage);
		if (pObject->getHealth() <= 0)
			m_pTank->addMadeKills(1);
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
	float factors = m_pTimer->getTimeFactor();
	float incX = m_speed*sin(m_rotate.z)*factors,
		incY = m_speed*cos(m_rotate.z)*factors;
	addX(-incX);
	addY(incY);
}