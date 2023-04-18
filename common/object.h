#ifndef _OBJECT_H
#define _OBJECT_H


//	const int UNIT_HEALTH = 100 HP; // Default unit health
//	const int UNIT_SPEED = 1 PX; // Default unit speed

#include "constants.h"
#include "image.h"
#include "audio.h"
#include "input.h"
#include "texturemanger.h"
#include <vector>

class Object : public Image
{
public:

	Object();
	~Object();
	virtual void initialize(uint16_t width, uint16_t height, uint8_t columns, uint8_t rows, bool animate,
		float updateDelay, float health, float speed, Texture* death, Texture* texture,
		Graphics* graphics);
	virtual void initialize(Texture* texture, TextureManger* textureManger, Audio* audio, Graphics* graphics);
	virtual void inputInitialize(Input* input, Key forward, Key back, Key right, Key left);
	virtual void audioInitialize(Sound forward, Sound back, Sound right, Sound left, Sound death);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void inputUpdate(float frameTime);
	virtual void release();
	virtual void healthIncrease(float increase) { m_objectInfo.health += increase; }
	virtual void damage(float value) { m_objectInfo.health -= value; }
	virtual void speedIncrease(float increase) { m_objectInfo.speed += increase; }
	virtual void speedDecrease(float decrease) { m_objectInfo.speed -= decrease; }
	virtual void setHealth(float health) { m_objectInfo.health = health; }
	virtual void setSpeed(float speed) { m_objectInfo.speed = speed; }
	virtual void setUpdateInput(bool updateInput) { m_updateInput = updateInput; }
	void setInput(bool input) { m_handleInput = input; }
	void setAudio(bool audio) { m_playAudio = audio; }
	virtual void setDeathMode();
	virtual void endFrame();

	virtual float getHealth() const { return m_objectInfo.health; }
	virtual float getSpeed() { return m_objectInfo.speed; }
	virtual bool getUpdateInput() { return m_updateInput; }
	const bool alive() const { return (m_objectInfo.health > 0) ? true : false; }
	virtual void executeForward(float frameTime) = 0;
	virtual void executeBack(float frameTime) = 0;
	virtual void executeRight(float frameTime) = 0;
	virtual void executeLeft(float frameTime) = 0;

protected:
	
	Audio* m_pAudio;
	Input* m_pInput;
	Texture* m_pDeath;
	ObjectInfo m_objectInfo;
	std::vector<Key> m_key;
	std::vector<Sound> m_sound;
	bool m_render;
	bool m_updateInput;
	bool m_alive;
	bool m_playAudio;
	bool m_handleInput;
};

inline V2 getObjectFocuusPoint(const Image& object1, const Image& object2)
{
	/*
	float x = object1.getCenterX();
	x = object1.getSin();
	x = object1.getHeight();
	x = object2.getWidth();
	*/
	V2 site;
	site.x = object1.getCenterX() + -object1.getSin()*(object1.getHeight() / 2) - object2.getWidth() / 2;
	site.y = object1.getCenterY() + (object1.getCos()*object1.getHeight() / 2) - (object2.getHeight() / 2);// +cos(ang)*object1.getHeight();
	return site;
}


#endif