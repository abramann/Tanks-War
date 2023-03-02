#include "tank.h"
#include "fileio.h"

Tank::Tank()
{
}

Tank::~Tank()
{
	release();
}

void Tank::initialize(int width, int height, float health, float speed, Map* map,Texture* death, Texture* texture, Graphics* graphics)
{
	Unit::initialize(width, height, 1, 1, false, 0, health, speed, map, death, texture, graphics);
}

void Tank::initialize(Map * map, TextureManger * textureManger, Texture * texture, Graphics * graphics)
{
	Unit::initialize(map, textureManger, texture, graphics);
	m_tankInfo = *textureManger->getTextureInfo(texture->getNumber())->tankInfo;
	m_pFire = m_pTextureManger->getTexture(m_tankInfo.fireTexture);
	m_fire.initialize(this, m_pMap, m_pTextureManger, m_pFire, m_pGraphics);
}

void Tank::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key)
{
	Unit::inputInitialize(input, forward_key, back_key, right_key, left_key);
	m_key.push_back(attack_key);
}

void Tank::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff, Effect shot_eff, Effect hit_eff)
{
	Object::audioInitialize(forward_eff, back_eff, right_eff, left_eff, death_eff);
	m_effect.push_back(shot_eff);
	m_effect.push_back(hit_eff);
}

void Tank::inputUpdate(float frameTime)
{
	Object::inputUpdate(frameTime);
	if (m_pInput->isKeyIn(m_key[TANK_KEYATTACK]))
		executeAttack(frameTime);
}

void Tank::update(float frameTime)
{
	Object::update(frameTime);
	m_fire.update(frameTime);
}

void Tank::draw()
{
	Object::draw();
	if (m_fire.active())
		m_fire.draw();
}

void Tank::release()
{
}

void Tank::executeForward(float frameTime)
{
	float x = getX() + (m_ObjectInfo.speed * m_sinA);
	float y = getY() - (m_ObjectInfo.speed * m_cosA);
	x = m_pMap->passX(x, getX(), getY(), getWidth());
	setX(x);
	y = m_pMap->passY(getX(), y, getY(), getHeight());
	setY(y);

	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_FORWARD]);
}

void Tank::executeBack(float frameTime)
{
	float x = getX() - (m_ObjectInfo.speed * m_sinA);
	float y = getY()+ (m_ObjectInfo.speed * m_cosA);
	x = m_pMap->passX(x, getX(), getY(), getWidth());
	setX(x);
	y = m_pMap->passY(getX(), y, getY(), getHeight());
	setY(y);

	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_BACK]);
}
constexpr auto TANK_ROTATE_AMOUNT = PI / 8;
void Tank::executeRight(float frameTime)
{
	if (m_spriteData.angle >= 3.14)
		m_spriteData.angle *= -1;
	
	m_spriteData.angle += TANK_ROTATE_AMOUNT;
}

void Tank::executeLeft(float frameTime)
{
	if (m_spriteData.angle <= -3.14)
		m_spriteData.angle *= -1;
	
	m_spriteData.angle -= TANK_ROTATE_AMOUNT;
}

void Tank::executeAttack(float frameTime)
{
	if (m_fire.active())
		return;

	m_fire.release(RELEASE_NORMAL);
/*	if (m_playAudio)
		m_pAudio->playCue(m_effect[TANK_EFFECTSHOT]);*/
}

void Tank::setTexture(Texture* texture, Texture* fire)
{
	Unit::setTexture(texture);
	m_pFire = fire;
}

void Tank::endFrame()
{
	Object::endFrame();
}
