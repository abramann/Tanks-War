// tank.cpp
// Author: abramann

#include "tank.h"
#include "game.h"
#include "bullet.h"
#include "map.h"
#include "texturemanger.h"
#include "texture.h"
#include "timer.h"

Tank::Tank() : m_bulletSpeed(logicNS::BULLET_SPEED), m_bulletDamage(logicNS::BULLET_DAMAGE), m_rotateAmount(logicNS::TANK_ROTATE_AMOUNT),
m_inflictedDamage(0)
{
	m_health = logicNS::HEALTH_TANK;
	m_velocity = logicNS::VELOCITY_TANK;
}

Tank::~Tank()
{
	m_pBullet.clear();
}

void Tank::initialize(std::string texture, const Game * pGame)
{
	m_pTimer = pGame->getTimer();
	Object::initialize(texture, pGame);
	if (m_tankTexture.empty())
		m_tankTexture = texture;
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

bool Tank::executeAttack()
{
	if (m_pBullet.size() == 0)
	{
		std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>(m_pGame, this);
		m_pBullet.push_back(pBullet);
		m_pAudio->play("tank-attack");
		return true;
	}

	return false;
}

void Tank::executeBack()
{
	float factors = m_pTimer->getTimeFactor() * m_pMap->getVelocityFactor(getSpace());;
	float x = m_position.x + (m_velocity * sin(m_rotate.z)*factors);
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y - (m_velocity * cos(m_rotate.z))*factors;
	m_position.y = m_pMap->passY(this, y);
	Object::executeForward();
}

void Tank::executeDie()
{
	Image::initialize("tank-destroy", nullptr, textureNS::TEXTURE_TANK_DESTROY_ROWS_COLUMNS, textureNS::TEXTURE_TANK_DESTROY_ROWS_COLUMNS,
		logicNS::UPDATE_DELAY_TANK_DESTORY);
	m_row = 1, m_column = 1;
	Object::executeDie();
}

void Tank::executeForward()
{
	float factors = m_pTimer->getTimeFactor() * m_pMap->getVelocityFactor(getSpace());;
	float x = m_position.x - (m_velocity * sin(m_rotate.z))*factors;
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y + (m_velocity * cos(m_rotate.z))*factors;
	m_position.y = m_pMap->passY(this, y);
	Object::executeForward();
}

void Tank::executeLeft()
{
	float factors = m_pTimer->getTimeFactor();
	if (m_rotate.z >= (logicNS::TANK_MAX_ANGLE - 0.01f))
		m_rotate.z *= -1;

	m_rotate.z += m_rotateAmount* factors;
	if (m_pMap->isCollided(this))
		m_rotate.z -= m_rotateAmount * factors;

	Object::executeLeft();
}

void Tank::executeRight()
{
	auto factors = m_pTimer->getTimeFactor();
	if (m_rotate.z <= -logicNS::TANK_MAX_ANGLE)
		m_rotate.z *= -1;

	m_rotate.z -= m_rotateAmount * factors;
	if (m_pMap->isCollided(this))
		m_rotate.z += m_rotateAmount * factors;
	Object::executeRight();
}

void Tank::executeAnimateRepeat()
{
	Image::initialize(m_tankTexture, 0);
	Image::executeAnimateRepeat();
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
	m_pAudio->play("tank-destroy");
}

void Tank::playSoundAttack()
{
}