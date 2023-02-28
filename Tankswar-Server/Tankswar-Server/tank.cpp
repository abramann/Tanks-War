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
}

void Tank::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, Texture* fire)
{
	m_pFire = fire;
	m_fire.initialize(this, m_pMap, &m_pFire[1], m_pFire, m_pGraphics);
	Unit::inputInitialize(input, forward_key, back_key, right_key, left_key);
	m_key.push_back(attack_key);
}

void Tank::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key)
{
	m_pFire = m_pTextureManger->getTexture(m_tankInfo.fireTexture);
	m_fire.initialize(this,m_pMap, m_pTextureManger, m_pFire, m_pGraphics);
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
	const float powSin = sign(m_sinA) * pow(m_sinA, 2);
	const float powCos = sign(m_cosA) * pow(m_cosA, 2);

	float extraX = 0;// powSin*((m_spriteData.angle / PI) + 0.5)*m_spriteData.width;
	extraX = getCenterX() + m_spriteData.height*powSin;
	float extraY = -powCos*_round(sin(m_spriteData.angle / 2))	* m_spriteData.height;
	float x = getCenterX() + (m_spriteData.height*powSin / 2) + (m_spriteData.width*powCos / 2) +(m_ObjectInfo.speed * powSin);
	float y = m_spriteData.y - (m_ObjectInfo.speed * powCos) - extraY;
	x = m_pMap->passX(x, m_spriteData.x, y);
	
	setX(x);

	y = m_pMap->passY(x, y, m_spriteData.y);
	y += extraY;
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_FORWARD]);
}

void Tank::executeBack(float frameTime)
{
	float powSin = sign(m_sinA) * pow(m_sinA, 2);
	float powCos = sign(m_cosA) * pow(m_cosA, 2);
	float extraX = 0;// powSin*((m_spriteData.angle / PI) + 0.5)	*m_spriteData.width;
	float extraY = powCos*cos(m_spriteData.angle / 2)	*m_spriteData.height;
	float x = m_spriteData.x - (m_ObjectInfo.speed * powSin) - extraX;
	float y = m_spriteData.y + (m_ObjectInfo.speed * powCos) - extraY;
	x = m_pMap->passX(x, m_spriteData.x, y);
	x += extraX;
	setX(x);
	y = m_pMap->passY(m_spriteData.x, y, m_spriteData.y);
	y += extraY;
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_BACK]);
}

void Tank::executeRight(float frameTime)
{
	m_spriteData.angle += PI / 4;
}

void Tank::executeLeft(float frameTime)
{
	m_spriteData.angle -= PI / 4;
}

void Tank::executeAttack(float frameTime)
{
	if (m_fire.active())
		return;

	m_fire.release(RELEASE_NORMAL);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[TANK_EFFECTSHOT]);
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

PlayerState Tank::getPlayerState() const
{
	PlayerState state;
	state.angle = getAngle();
	state.health = getHealth();
	state.x = getX();
	state.y = getY();
	return state;
}


