#include "object.h"
#include "fileio.h"

Object::Object() :  m_playAudio(false),
	m_handleInput(false), m_alive(true), m_render(true)
{
}

Object::~Object()
{
	release();
}

void Object::initialize(
	uint16_t width, uint16_t height, uint8_t columns, uint8_t rows, bool animate,
	float updateDelay, float health, float speed, Texture* death, Texture* texture,
	Graphics* graphics)
{
	setSpeed(speed);
	setHealth(health);
	m_pDeath = death;
	Image::initialize(width, height, columns, rows, false, updateDelay, texture, graphics);
}

void Object::initialize(Texture * texture, TextureManger* textureManger, Audio* audio, Graphics * graphics)
{
	Image::initialize(texture, textureManger, graphics);
	m_pAudio = audio;
	m_objectInfo = *m_pTextureManger->getTextureInfo(m_pTexture->getNumber())->objectInfo;
	m_pDeath = m_pTextureManger->getTexture(m_objectInfo.deathTexture);
}

void Object::inputInitialize(Input* input, Key forward, Key back, Key right, Key left)
{
	m_pInput = input;
	setInput(true);
	m_key.push_back(forward);
	m_key.push_back(back);
	m_key.push_back(right);
	m_key.push_back(left);
}

void Object::audioInitialize(Sound forward, Sound back, Sound right, Sound left, Sound death)
{
	m_playAudio = true;
	m_sound.push_back(forward);
	m_sound.push_back(back);
	m_sound.push_back(right);
	m_sound.push_back(left);
	m_sound.push_back(death);
}

void Object::update(float frameTime)
{
	//if(m_alive)
	//	if (m_objectInfo.health <= 0)
	//		setDeathMode();

	if (m_handleInput)
		inputUpdate(frameTime);

	mathUpdate();
	Image::update(frameTime);
}

void Object::draw()
{
	if (m_render)
		Image::draw();
}

void Object::inputUpdate(float frameTime)
{
	if (m_pInput->isKeyIn(m_key[KEY_OBJECT_FORWARD]))
		executeForward(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_OBJECT_BACK]))
		executeBack(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_OBJECT_RIGHT]))
		executeRight(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_OBJECT_LEFT]))
		executeLeft(frameTime);
}

void Object::release()
{
}

void Object::setDeathMode()
{
	m_spriteData;
	m_alive = false;
	setInput(false);
	setTexture(m_pDeath);
	setDefaultImageInfo();
	setDefaultColumnRow();
	setAnimate(true);
}

void Object::endFrame()
{
	if (!m_alive)
	{
		setAudio(false);
		setNullSpriteData();
	}
	
}

