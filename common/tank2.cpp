#include "tank2.h"
#include "game.h"
#include "bullet.h"
#include "texturemanger.h"
#include "texture.h"

constexpr auto HEALTH_TANK = 100;
constexpr auto VELOCITY_TANK = 10;
constexpr auto UPDATE_DELAY_TANK_DESTORY = 100;
const auto TANK_MAX_ANGLE = PI - 0.01f;

Tank2::Tank2() : m_bulletSpeed(BULLET_SPEED), m_bulletDamage(BULLET_DAMAGE)
{
	m_health = HEALTH_TANK;
	m_velocity = VELOCITY_TANK;
}

Tank2::~Tank2()
{
	m_pBullet.clear();
	//for (auto& pBullet : m_pBullet)
		//safeDelete(pBullet);
}

void Tank2::initialize(Texture* texture, const Game * game)
{
	m_pTextureManger = game->getTextureManger();
	m_pTimer = game->getTimer();
	Object2::initialize(texture, game);
}

void Tank2::update(float frameTime)
{
	Object2::update(frameTime);
	int8 i = 0;
	for (int i = 0; i < m_pBullet.size();i++)
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

void Tank2::draw()
{
	Object2::draw();
	for (int i = 0; i < m_pBullet.size();i++)
		m_pBullet[i]->draw();
}

void Tank2::executeAttack()
{
	if (m_pBullet.size() == 0)
	{
		std::shared_ptr<Bullet> pBullet = std::make_shared<Bullet>(m_pGame, this);
		m_pBullet.push_back(pBullet);
	}
}

void Tank2::executeBack(float frameTime)
{
	float x = m_position.x + (m_velocity * sin(m_rotate.z));
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y - m_velocity * cos(m_rotate.z);
	m_position.y = m_pMap->passY(this, y);
	Object2::executeForward();
}

void Tank2::executeDie()
{
	m_pTexture = m_pTextureManger->getTexture(TEXTURE_TANK_DESTROY);
	Image2::initialize(m_pTexture, m_pGame, TEXTURE_TANK_DESTROY_ROWS_COLUMNS, TEXTURE_TANK_DESTROY_ROWS_COLUMNS, UPDATE_DELAY_TANK_DESTORY);
	Object2::executeDie();
}

void Tank2::executeForward(float frameTime)
{
	float x = m_position.x - m_velocity * sin(m_rotate.z);
	m_position.x = m_pMap->passX(this, x);
	float y = m_position.y + m_velocity * cos(m_rotate.z);
	m_position.y = m_pMap->passY(this, y);
	Object2::executeForward();
}

void Tank2::executeLeft(float frameTime)
{
	if (m_rotate.z >= (TANK_MAX_ANGLE - 0.01f))
		m_rotate.z *= -1;

	m_rotate.z += TANK_ROTATE_AMOUNT;
	if(m_pMap->isCollided(this))
		m_rotate.z -= TANK_ROTATE_AMOUNT;

	Object2::executeLeft();
}

void Tank2::executeRight(float frameTime)
{
	if (m_rotate.z <= -TANK_MAX_ANGLE)
		m_rotate.z *= -1;

	m_rotate.z -= TANK_ROTATE_AMOUNT;
	if (m_pMap->isCollided(this))
		m_rotate.z += TANK_ROTATE_AMOUNT;
	Object2::executeRight();
}

void Tank2::playSoundForward()
{
}

void Tank2::playSoundBack()
{
}

void Tank2::playSoundRight()
{
}

void Tank2::playSoundLeft()
{
}

void Tank2::playSoundDie()
{
}

void Tank2::playSoundAttack()
{
}