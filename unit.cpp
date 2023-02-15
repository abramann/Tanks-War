#include "unit.h"

Unit::Unit() : m_health(0), m_speed(0), m_playAudio(false),
	m_handleInput(false), m_deathExecuted(false),
	m_render(true)
{
	m_pAudio = new Audio;
}

Unit::~Unit()
{
	release();
}

void Unit::initialize(int width, int height, int columns, int rows, bool animate, float updateDelay, float health, float speed, Map* map, TextureManger* death, TextureManger* textureManger, Graphics* graphics)
{
	m_health = health;
	m_speed = speed;
	m_pMap = map;
	m_pDeath = death;
	m_pAudio->initialize();
	m_death.initialize(0, 0, 1, 1, true, 200, m_pDeath, m_pGraphics);
	Image::initialize(width, height, columns, rows, false, updateDelay, textureManger, graphics);
}

void Unit::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key)
{
	m_handleInput = true;
	m_pInput = input;
	m_key.push_back(forward_key);
	m_key.push_back(back_key);
	m_key.push_back(right_key);
	m_key.push_back(left_key);
}

void Unit::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff)
{
	m_playAudio = true;
	m_effect.push_back(forward_eff); // EFFECTFORWARD
	m_effect.push_back(back_eff);	// EFFECTBACK
	m_effect.push_back(right_eff);  // EFFECTRIGHT
	m_effect.push_back(left_eff);	// EFFECTLEFT
	m_effect.push_back(death_eff);	// EFFECTDEATH
}

void Unit::update(float frameTime)
{
	if (!m_deathExecuted)
	{
		if (m_health <= 0)
			executeDeath();

		mathUpdate();
	}
	else if (m_column == m_spriteData.columns & m_row == m_spriteData.rows)
	{
		m_render = false;
		return;
	}
	if (m_handleInput)
		inputUpdate(frameTime);

	if (m_playAudio)
		m_pAudio->run();

	Image::update(frameTime);
}

void Unit::draw()
{
	if (m_render)
		Image::draw();
}

void Unit::inputUpdate(float frameTime)
{
	if (m_pInput->isKeyIn(m_key[KEY_FORWARD]))
		executeForward(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_BACK]))
		executeBack(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_RIGHT]))
		executeRight(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_LEFT]))
		executeLeft(frameTime);
}

void Unit::executeForward(float frameTime)
{
	int y = m_spriteData.y - (m_speed * frameTime);
	y = m_pMap->passY(m_spriteData.x + m_spriteData.width / 2, y, m_spriteData.y);
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[KEY_FORWARD]);
}

void Unit::executeBack(float frameTime)
{
	int y = m_spriteData.y + (m_speed * frameTime) + m_spriteData.height;
	y = m_pMap->passY(m_spriteData.x, y, m_spriteData.y) - m_spriteData.height;
	setY(y);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_BACK]);
}

void Unit::executeRight(float frameTime)
{
	int x = m_spriteData.x + (frameTime*m_speed) + m_spriteData.width;
	x = m_pMap->passX(x, m_spriteData.x, m_spriteData.y) - m_spriteData.width;
	setX(x);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_RIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	int newX = m_spriteData.x - (m_speed * frameTime);
	newX = m_pMap->passX(newX, m_spriteData.x, m_spriteData.y);
	setX(newX);
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_LEFT]);
}

void Unit::executeDeath()
{
	m_handleInput = false;
	m_deathExecuted = true;
	auto currentX = m_spriteData.x;
	auto currentY = m_spriteData.y;
	m_spriteData = m_death.getSpriteData();
	m_spriteData.x = currentX;
	m_spriteData.y = currentY;
	
	m_animate = m_death.isAnimated();
	
	if (m_playAudio)
		m_pAudio->playCue(m_effect[EFFECT_DEATH]);
	
}

void Unit::release()
{
	SAFE_DELETE(m_pAudio);
}

