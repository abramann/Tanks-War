// tank.cpp
// Author: abramann

#include "tank.h"
#include "game.h"
#include "bullet.h"
#include "texturemanger.h"
#include "texture.h"
#include "map.h"
#include "timer.h"

Tank::Tank() : m_bulletSpeed(logicNS::BULLET_SPEED), m_bulletDamage(logicNS::BULLET_DAMAGE), m_rotateAmount(logicNS::TANK_ROTATE_AMOUNT)
{
	m_health = logicNS::HEALTH_TANK;
	m_velocity = logicNS::VELOCITY_TANK;
}

Tank::~Tank()
{
	m_pBullet.clear();
}

void Tank::initialize(Texture* texture, const Game * game)
{
	m_pTextureManger = game->getTextureManger();
	m_pTimer = game->getTimer();
	Object::initialize(texture, game);
}

void Tank::update()
{
	Object::update();
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		Bullet* pBullet = m_pBullet[i].get();
		if (pBullet->isFinished())
			m_pBullet.erase(std::next(m_pBullet.begin(), i--));
		else
			pBullet->update();
	}
}

void Tank::draw()
{
	Object::draw();
	for (size_t i = 0; i < m_pBullet.size(); i++)
		m_pBullet[i]->draw();
}

void Tank::executeAttack()
{
	if (m_pBullet.size() == 0)
	{
		std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>(m_pGame, this);
		m_pBullet.push_back(pBullet);
	}
}

void Tank::executeBack()
{
	auto timeFactor = m_pTimer->getTimeFactor();
	float x = m_position.x + (m_velocity * sin(m_rotate.z)*timeFactor);
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y - (m_velocity * cos(m_rotate.z))*timeFactor;
	m_position.y = m_pMap->passY(this, y);
	Object::executeForward();
}

void Tank::executeDie()
{
	m_pTexture = m_pTextureManger->getTexture(TEXTURE_TANK_DESTROY);
	Image::initialize(m_pTexture, m_pGame, textureNS::TEXTURE_TANK_DESTROY_ROWS_COLUMNS, textureNS::TEXTURE_TANK_DESTROY_ROWS_COLUMNS,
		logicNS::UPDATE_DELAY_TANK_DESTORY);
	Object::executeDie();
}

void Tank::executeForward()
{
	auto timeFactor = m_pTimer->getTimeFactor();
	float x = m_position.x - (m_velocity * sin(m_rotate.z))*timeFactor;
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y + (m_velocity * cos(m_rotate.z))*timeFactor;
	m_position.y = m_pMap->passY(this, y);
	Object::executeForward();
}

void Tank::executeLeft()
{
	auto timeFactor = m_pTimer->getTimeFactor();
	if (m_rotate.z >= (logicNS::TANK_MAX_ANGLE - 0.01f))
		m_rotate.z *= -1;

	m_rotate.z += m_rotateAmount* timeFactor;
	if (m_pMap->isCollided(this))
		m_rotate.z -= m_rotateAmount * timeFactor;

	Object::executeLeft();
}

void Tank::executeRight()
{
	auto timeFactor = m_pTimer->getTimeFactor();
	if (m_rotate.z <= -logicNS::TANK_MAX_ANGLE)
		m_rotate.z *= -1;

	m_rotate.z -= m_rotateAmount * timeFactor;
	if (m_pMap->isCollided(this))
		m_rotate.z += m_rotateAmount * timeFactor;
	Object::executeRight();
}

void Tank::playSoundForward()
{
}

void Tank::playSoundBack()
{
}

void Tank::playSoundRight()
{
}

void Tank::playSoundLeft()
{
}

void Tank::playSoundDie()
{
}

void Tank::playSoundAttack()
{
}