#include "bullet.h"
#include "game.h"
#include "tank2.h"
#include "texturemanger.h"
#include "texture.h"

Bullet::Bullet(const Game * game, const Tank2 * tank) : m_pTank(tank), m_hit(false), m_finish(false)
{
	m_pGame = game;
	m_pMap = game->getMap();
	m_pAudio = game->getAudio();
	m_pTextureManger = game->getTextureManger();
	Texture* pTexture = m_pTextureManger->getTexture(TEXTURE_BULLET);
	Image2::initialize(pTexture, game);
	executeLaunch();
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::update(float frameTime)
{
	//m_position = getBulletLaunchPosition();
	//m_rotate = m_pTank->getRotate();
	Image2::update(frameTime);
	if (m_finish)
		return;
	if (!m_hit)
	{
		executeLaunching(frameTime);
		if (isCollided())
			executeHit();
	}

}

V3 Bullet::getBulletLaunchPosition()
{
	Space s = Map2::getImageSpace(m_pTank);
	V3 position;
	position.x = (s.v4.x + (s.v3.x - s.v4.x) / 2);
	position.y = (s.v4.y + (s.v3.y - s.v4.y) / 2);
	return position;
}

bool Bullet::isCollided()
{
	return m_pMap->isCollided(this);
	if (m_position.x == m_pMap->passX(this, 0) ||
		m_position.y == m_pMap->passY(this, 0))
		return true;
}

void Bullet::executeLaunch()
{
	m_speed = 5;// m_pTank->getBulletSpeed();
	m_damage = m_pTank->getBulletDamage();
	m_rotate = m_pTank->getRotate();
	V3 pos = m_pTank->getPosition();
	float f1 = 1.0f - abs((0.636619772f *m_rotate.z));
	m_position = getBulletLaunchPosition();
	executeLaunching(22);
	executeLaunching(22);
	executeLaunching(22);
}

void Bullet::executeHit()
{
	m_hit = true;
	Texture* pTexture = m_pTextureManger->getTexture(TEXTURE_BULLET_DESTROY);
	Image2::initialize(pTexture, m_pGame, TEXTURE_BULLET_ROWS_COLUMNS, TEXTURE_BULLET_ROWS_COLUMNS, UPDATE_DELAY_BULLET);
#ifdef _SERVER_BUILD
	Object2* pObject = m_pMap->getObject(m_position);
	if (pObject)
		pObject->damage(m_damage);
#endif
}

void Bullet::executeAnimateRepeat()
{
	m_finish = true;
}

void Bullet::executeLaunching(float frameTime)
{
	float incX = m_speed*sin(m_rotate.z),
		incY = m_speed*cos(m_rotate.z);
	m_position;
	addX(-incX);
	addY(incY);
}
