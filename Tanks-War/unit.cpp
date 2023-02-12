#include "unit.h"

Unit::Unit() : m_health(0), m_speed(0), m_sinA(0),
	m_cosA(0), m_playAudio(false), m_handleInput(false), m_deathExecuted(false),
	m_render(true)
{
	m_audio = new Audio;
}

Unit::~Unit()
{
	release();
}

void Unit::initialize(int width, int height, int columns, int rows, bool animate, float updateDelay, float health, float speed, Image* death, Map* map, TextureManger* textureManger, Graphics* graphics)
{
	m_health = health;
	m_speed = speed;
	m_death = death;
	m_map = map;
	m_audio->initialize();
	Image::initialize(width, height, columns, rows, false, updateDelay, textureManger, graphics);
	if (IsBadReadPtr(death,sizeof(Image)))
		return;
}

void Unit::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key)
{
	m_handleInput = true;
	m_input = input;
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
		m_audio->run();

	Image::update(frameTime);
}

void Unit::draw()
{
	if (m_render)
		Image::draw();
}

void Unit::inputUpdate(float frameTime)
{
	if (m_input->isKeyIn(m_key[KEYFORWARD]))
		executeForward(frameTime);
	if (m_input->isKeyIn(m_key[KEYBACK]))
		executeBack(frameTime);
	if (m_input->isKeyIn(m_key[KEYRIGHT]))
		executeRight(frameTime);
	if (m_input->isKeyIn(m_key[KEYLEFT]))
		executeLeft(frameTime);
}

void Unit::executeForward(float frameTime)
{
	int y = m_spriteData.y - (m_speed * frameTime);
	y = m_map->passY(m_spriteData.x + m_spriteData.width / 2, y, m_spriteData.y);
	setY(y);
	if (m_playAudio)
		m_audio->playCue(m_effect[EFFECTFORWARD]);
}

void Unit::executeBack(float frameTime)
{
	int y = m_spriteData.y + (m_speed * frameTime) + m_spriteData.height;
	y = m_map->passY(m_spriteData.x, y, m_spriteData.y) - m_spriteData.height;
	setY(y);
	if (m_playAudio)
		m_audio->playCue(m_effect[EFFECTBACK]);
}

void Unit::executeRight(float frameTime)
{
	int x = m_spriteData.x + (frameTime*m_speed) + m_spriteData.width;
	x = m_map->passX(x, m_spriteData.x, m_spriteData.y) - m_spriteData.width;
	setX(x);
	if (m_playAudio)
		m_audio->playCue(m_effect[EFFECTRIGHT]);
}

void Unit::executeLeft(float frameTime)
{
	int newX = m_spriteData.x - (m_speed * frameTime);
	newX = m_map->passX(newX, m_spriteData.x, m_spriteData.y);
	setX(newX);
	if (m_playAudio)
		m_audio->playCue(m_effect[EFFECTLEFT]);
}

void Unit::executeDeath()
{
	return;
	m_handleInput = false;
	m_deathExecuted = true;
	int currentX = m_spriteData.x;
	int currentY = m_spriteData.y;
	m_spriteData = m_death->getSpriteData();
	m_spriteData.x = currentX;
	m_spriteData.y = currentY;
	
	m_animate = m_death->isAnimated();
	
	if (m_playAudio)
		m_audio->playCue(m_effect[EFFECTDEATH]);
	
}

void Unit::release()
{
	SAFE_DELETE(m_audio);
	//SAFE_DELETE_ARRAY(m_key);
	//SAFE_DELETE_ARRAY(m_effect);
}

void Unit::mathUpdate()
{
	m_sinA = sin(getAngle());
	if (m_sinA > 1)
		m_sinA = 1;
	else if (m_sinA < -1)
		m_sinA = -1;
	else if (abs(m_sinA) < 0.0001)
		m_sinA = 0;
	m_cosA = cos(getAngle());
	if (m_cosA > 1)
		m_cosA = 1;
	else if (m_cosA < -1)
		m_cosA = -1;
	else if (abs(m_cosA) < 0.0001)
		m_cosA = 0;
}

