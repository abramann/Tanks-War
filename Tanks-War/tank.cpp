#include "tank.h"

Tank::Tank()
{
}

Tank::~Tank()
{
	release();
}

void Tank::initialize(int width, int height, float health, float speed, Image* death, Map* map, TextureManger* textureManger, Graphics* graphics)
{
	Unit::initialize(width, height, 1, 1, false, 0, health, speed, death, map, textureManger, graphics);
}

void Tank::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, TextureManger* _fireTex)
{
	fireTex = _fireTex;
	//fire.initialize(fireTex, m_graphics);
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
	if (m_input->isKeyIn(m_key[TANK_KEYATTACK]))
	{
		executeAttack(frameTime);
	}
}

void Tank::release()
{
	//SAFE_DELETE(rect);
}

void Tank::executeForward(float frameTime)
{

	float powSin = getSinSign() * pow(m_sinA, 2);
	float powCos = getCosSign() * pow(m_cosA, 2);

	float extraX = 0;// powSin*((m_spriteData.angle / PI) + 0.5)	*m_spriteData.width;

	float extraY = -powCos*sin(m_spriteData.angle / 2)	*m_spriteData.height;
	float x = m_spriteData.x + (m_speed * powSin) - extraX;
	float y = m_spriteData.y - (m_speed * powCos) - extraY;
	x = m_map->passX(x, m_spriteData.x, y);
	x += extraX;
	setX(x);

	y = m_map->passY(x, y, m_spriteData.y);
	y += extraY;
	setY(y);

	/*
	m_spriteData.x += cosA * speed * frameTime;
	m_spriteData.y += sinA * speed * frameTime;
	*/
}

void Tank::executeBack(float frameTime)
{
	float powSin = getSinSign() * pow(m_sinA, 2);
	float powCos = getCosSign() * pow(m_cosA, 2);

	float extraX = 0;// powSin*((m_spriteData.angle / PI) + 0.5)	*m_spriteData.width;
	float extraY = powCos*cos(m_spriteData.angle / 2)	*m_spriteData.height;
	float x = m_spriteData.x - (m_speed * powSin) - extraX;
	float y = m_spriteData.y + (m_speed * powCos) - extraY;
	x = m_map->passX(x, m_spriteData.x, y);
	x += extraX;
	setX(x);
	y = m_map->passY(m_spriteData.x, y, m_spriteData.y);
	y += extraY;
	setY(y);
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
	//setFrameRect(frame[TANK_FRAMEATTACK], TANK_ATTACKDELAY);
	m_audio->playCue(m_effect[TANK_EFFECTSHOT]);
}

void Tank::setTexture(TextureManger* _textureManger, TextureManger* _fireTex)
{
	Unit::setTexture(_textureManger);
	fireTex = _fireTex;
}
