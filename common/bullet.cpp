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
	Image2::update(frameTime);
	if (m_finish)
		return;
	if(!m_hit)
	{
		executeLaunching(frameTime);
		if (m_position.x == m_pMap->passX(this, 0) ||
			m_position.y == m_pMap->passY(this, 0))
	
	executeHit();
	}

}

void Bullet::executeLaunch()
{
	m_speed = m_pTank->getBulletSpeed();
	m_damage = m_pTank->getBulletDamage();
	m_rotate = m_pTank->getRotate();
	V3 pos = m_pTank->getPosition();
	/*int16 width = m_pTank->getWidth(),
		height = m_pTank->getHeight();
	float f1 = (1 + (0.3183098f)*m_rotate.z);
	float f2 = -sin(m_rotate.z);

	m_position.x = pos.x + 30 + 30*f2,
		m_position.y = pos.y + f1*height,
		m_position.z = pos.z;
	*/
	
	Space s = Map2::getImageSpace(m_pTank);
	m_position.x = s.v4.x + (s.v3.x - s.v4.x) / 2;
	m_position.y = s.v4.y + (s.v3.y - s.v4.y) / 2;
	m_position.z = 0;
	
}

void Bullet::executeHit()
{
	m_hit = true;
	Texture* pTexture = m_pTextureManger->getTexture(TEXTURE_BULLET_DESTROY);
	Image2::initialize(pTexture, m_pGame, TEXTURE_BULLET_ROWS_COLUMNS, TEXTURE_BULLET_ROWS_COLUMNS, UPDATE_DELAY_BULLET);
	Object2* pObject = m_pMap->getObject(m_position);
	if (pObject)
		pObject->addHealth(-m_damage);

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
