#include "object.h"
#include "fileio.h"

Object::Object() :  m_playAudio(false),
	m_handleInput(false), m_alive(true), m_render(true)
{
	m_pAudio = new Audio;
}

Object::~Object()
{
	release();
}

void Object::initialize(uint16_t width, uint16_t height, uint8_t columns, uint8_t rows, bool animate, float updateDelay, float health, float speed, Texture* death, Texture* texture, Graphics* graphics)
{
	m_pObjectInfo = new ObjectInfo;
	setSpeed(speed);
	setHealth(health);
	m_pDeath = death;
	m_pAudio->initialize();
	Image::initialize(width, height, columns, rows, false, updateDelay, texture, graphics);
}

void Object::initialize(Texture * texture, TextureManger* textureManger, Graphics * graphics)
{
	Image::initialize(texture, textureManger, graphics);
	m_pObjectInfo = m_pTextureManger->getTextureInfo(m_pTexture->getNumber())->objectInfo;
	m_pDeath = m_pTextureManger->getTexture(m_pObjectInfo->deathTexture);

}

void Object::inputInitialize(Input* input, Key forward_key, Key back_key, Key right_key, Key left_key)
{
	m_pInput = input;
	setInput(true);
	m_key.push_back(forward_key);
	m_key.push_back(back_key);
	m_key.push_back(right_key);
	m_key.push_back(left_key);
}

void Object::audioInitialize(Effect forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff)
{
	setAudio(true);
	m_effect.push_back(forward_eff);
	m_effect.push_back(back_eff);
	m_effect.push_back(right_eff);
	m_effect.push_back(left_eff);
	m_effect.push_back(death_eff);
}

void Object::update(float frameTime)
{
	if(m_alive)
		if (m_pObjectInfo->health <= 0)
			setDeathMode();

	if (m_handleInput)
		inputUpdate(frameTime);

	if (m_playAudio)
		m_pAudio->run();

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
	if (m_pInput->isKeyIn(m_key[KEY_FORWARD]))
		executeForward(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_BACK]))
		executeBack(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_RIGHT]))
		executeRight(frameTime);
	if (m_pInput->isKeyIn(m_key[KEY_LEFT]))
		executeLeft(frameTime);
		
}

void Object::release()
{
	SAFE_DELETE(m_pAudio);
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

