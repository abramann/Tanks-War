#include "tank.h"

Tank::Tank()
{
}

Tank::~Tank()
{
	release();
}

void Tank::initialize(int width, int height, float health, float speed, Map* map,TextureManger* death, TextureManger* textureManger, Graphics* graphics)
{
	Unit::initialize(width, height, 1, 1, false, 0, health, speed, map, death, textureManger, graphics);
}

void Tank::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, TextureManger* fire)
{
	m_pFire = fire;
	m_fireData.speed = DEFAULT_FIRE_SPEED;
	m_fireData.healthDecrease = DEFAULT_FIRE_HEALTHDECREASE;
	m_fire.initialize(m_fireData, m_pMap, m_pFire, m_pGraphics);
	Unit::inputInitialize(input, forward_key, back_key, right_key, left_key);
	m_key.push_back(attack_key);
}

void Tank::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff, Effect shot_eff, Effect hit_eff)
{
	Unit::audioInitialize(forward_eff, back_eff, right_eff, left_eff, death_eff);
	m_effect.push_back(shot_eff);
	m_effect.push_back(hit_eff);
}

void Tank::inputUpdate(float frameTime)
{
	Unit::inputUpdate(frameTime);
	if (m_pInput->isKeyIn(m_key[TANK_KEYATTACK]))
		executeAttack(frameTime);
}

void Tank::update(float frameTime)
{
	Unit::update(frameTime);
	m_fire.update(frameTime);
}

void Tank::draw()
{
	Unit::draw();
	m_fire.draw();
}

void Tank::release()
{
}

void Tank::executeForward(float frameTime)
{
	float powSin = sign(m_sinA) * pow(m_sinA, 2);
	float powCos = sign(m_cosA) * pow(m_cosA, 2);

	float extraX = 0;// powSin*((m_spriteData.angle / PI) + 0.5)	*m_spriteData.width;

	float extraY = -powCos*sin(m_spriteData.angle / 2)	* m_spriteData.height;
	float x = m_spriteData.x + (m_speed * powSin) - extraX;
	float y = m_spriteData.y - (m_speed * powCos) - extraY;
	x = m_pMap->passX(x, m_spriteData.x, y);
	x += extraX;
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
	float x = m_spriteData.x - (m_speed * powSin) - extraX;
	float y = m_spriteData.y + (m_speed * powCos) - extraY;
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
	m_fire.release(*this, RELEASE_NORMAL);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[TANK_EFFECTSHOT]);
}

void Tank::setTexture(TextureManger* textureManger, TextureManger* fire)
{
	Unit::setTexture(textureManger);
	m_pFire = fire;
}
