// tank.cpp
// Author: abramann

#include "tank.h"
#include "game.h"
#include "bullet.h"
#include "texturemanger.h"
#include "texture.h"
#include "map.h"

constexpr auto HEALTH_TANK = 100;
constexpr auto VELOCITY_TANK = 10;
constexpr auto UPDATE_DELAY_TANK_DESTORY = 100;
const auto TANK_MAX_ANGLE = PI - 0.01f;

Tank::Tank() : m_bulletSpeed(BULLET_SPEED), m_bulletDamage(BULLET_DAMAGE)
{
	m_health = HEALTH_TANK;
	m_velocity = VELOCITY_TANK;
}

Tank::~Tank()
{
	m_pBullet.clear();
	//for (auto& pBullet : m_pBullet)
		//safeDelete(pBullet);
}

void Tank::initialize(Texture* texture, const Game * game)
{
	m_pTextureManger = game->getTextureManger();
	m_pTimer = game->getTimer();
	Object::initialize(texture, game);
}

void Tank::update(float frameTime)
{
	Object::update(frameTime);
	int8 i = 0;
	for (size_t i = 0; i < m_pBullet.size(); i++)
	{
		Bullet* pBullet = m_pBullet[i].get();
		if (pBullet->isFinished())
		{
			//safeDelete(pBullet);
			m_pBullet.erase(std::next(m_pBullet.begin(), i));
		}
		else
			pBullet->update(frameTime);

		i++;
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

void Tank::executeBack(float frameTime)
{
	float x = m_position.x + (m_velocity * sin(m_rotate.z));
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y - m_velocity * cos(m_rotate.z);
	m_position.y = m_pMap->passY(this, y);
	Object::executeForward();
}

void Tank::executeDie()
{
	m_pTexture = m_pTextureManger->getTexture(TEXTURE_TANK_DESTROY);
	Image::initialize(m_pTexture, m_pGame, textureNS::TEXTURE_TANK_DESTROY_ROWS_COLUMNS, textureNS::TEXTURE_TANK_DESTROY_ROWS_COLUMNS, UPDATE_DELAY_TANK_DESTORY);
	Object::executeDie();
}

void Tank::executeForward(float frameTime)
{
	float x = m_position.x - m_velocity * sin(m_rotate.z);
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y + m_velocity * cos(m_rotate.z);
	m_position.y = m_pMap->passY(this, y);
	Object::executeForward();
}

void Tank::executeLeft(float frameTime)
{
	if (m_rotate.z >= (TANK_MAX_ANGLE - 0.01f))
		m_rotate.z *= -1;

	m_rotate.z += logicNS::TANK_ROTATE_AMOUNT;
	if (m_pMap->isCollided(this))
		m_rotate.z -= logicNS::TANK_ROTATE_AMOUNT;

	Object::executeLeft();
}

void Tank::executeRight(float frameTime)
{
	if (m_rotate.z <= -TANK_MAX_ANGLE)
		m_rotate.z *= -1;

	m_rotate.z -= logicNS::TANK_ROTATE_AMOUNT;
	if (m_pMap->isCollided(this))
		m_rotate.z += logicNS::TANK_ROTATE_AMOUNT;
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