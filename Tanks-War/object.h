#ifndef _OBJECT_H
#define _OBJECT_H

enum OBJECT_KEYS {
	KEY_FORWARD,
	KEY_BACK,
	KEY_RIGHT,
	KEY_LEFT,
};

enum OBJECT_EFFECTS {
	EFFECT_FORWARD,
	EFFECT_BACK,
	EFFECT_RIGHT,
	EFFECT_LEFT,
	EFFECT_DEATH
};

#define FRAMEDEATH FRAMEEND

//	const int UNIT_HEALTH = 100 HP; // Default unit health
//	const int UNIT_SPEED = 1 PX; // Default unit speed

#include "always.h"
#include "image.h"
#include "audio.h"
#include "input.h"
#include <vector>

class Object : public Image
{
public:

	Object();
	~Object();
	virtual void initialize(int width, int height, int columns, int rows, bool animate, float updateDelay, float health, float speed, TextureManger* death, TextureManger* textureManger, Graphics* graphics);
	virtual void inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key);
	virtual void audioInitialize(Effect _forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void inputUpdate(float frameTime);
	virtual void release();
	virtual void healthIncrease(float increase) { m_health += increase; }
	virtual void damage(float value) { m_health -= value; }
	virtual void speedIncrease(float increase) { m_speed += increase; }
	virtual void speedDecrease(float decrease) { m_speed -= decrease; }
	virtual void setHealth(float health) { m_health = health; }
	virtual void setSpeed(float speed) { m_speed = speed; }
	virtual void setUpdateInput(bool updateInput) { m_updateInput = updateInput; }
	void setInput(bool input) { m_handleInput = input; }
	void setAudio(bool audio) { m_playAudio = audio; }
	virtual void setDeathMode();
	virtual void endFrame();

	virtual float getHealth() { return m_health; }
	virtual float getSpeed() { return m_speed; }
	virtual bool getUpdateInput() { return m_updateInput; }
	bool alive() { return (m_health > 0) ? true : false; }
	virtual void executeForward(float frameTime) = 0;
	virtual void executeBack(float frameTime) = 0;
	virtual void executeRight(float frameTime) = 0;
	virtual void executeLeft(float frameTime) = 0;
	

	float m_health, m_speed;


protected:
	
	Audio* m_pAudio;
	Input* m_pInput;
	TextureManger* m_pDeath;
	std::vector<Key> m_key;
	std::vector<Effect> m_effect;

	bool m_render;
	bool m_updateInput;
	bool m_alive;
	bool m_playAudio;
	bool m_handleInput;
};

inline V2 getObjectFocuusPoint(const Image& object1, const Image& object2)
{
	V2 site;
	site.x = object1.getCenterX() + -object1.getSin()*(object1.getHeight() / 2) - object2.getWidth() / 2;
	site.y = object1.getCenterY() + (object1.getCos()*object1.getHeight() / 2) - (object2.getHeight() / 2);// +cos(ang)*object1.getHeight();
	return site;
}


#endif