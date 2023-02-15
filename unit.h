#ifndef _UNIT_H
#define _UNIT_H

enum UNIT_KEYS {
	KEY_FORWARD,
	KEY_BACK,
	KEY_RIGHT,
	KEY_LEFT,
};

enum UNIT_EFFECTS {
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
#include "map.h"

class Unit : public Image
{
public:

	Unit();
	~Unit();
	virtual void initialize(int width, int height, int columns, int rows, bool animate, float updateDelay, float health, float speed, Map* map, TextureManger* death, TextureManger* textureManger, Graphics* graphics);
	virtual void inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key);
	virtual void audioInitialize(Effect _forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff);
	virtual void update(float frameTime);
	virtual void draw();
	virtual void inputUpdate(float frameTime);
	virtual void release();
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeDeath();
	virtual void healthIncrease(float increase) { m_health += increase; }
	virtual void healthDecrease(float decrease) { m_health -= decrease; }
	virtual void speedIncrease(float increase) { m_speed += increase; }
	virtual void speedDecrease(float decrease) { m_speed -= decrease; }

	virtual void setHealth(float health) { m_health = health; }
	virtual void setSpeed(float speed) { m_speed = speed; }
	virtual void setUpdateInput(bool updateInput) { m_updateInput = updateInput; }

	virtual float getHealth() { return m_health; }
	virtual float getSpeed() { return m_speed; }

	virtual bool getUpdateInput() { return m_updateInput; }
	
protected:
	
	Audio* m_pAudio;
	Input* m_pInput;
	TextureManger* m_pDeath;
	Image m_death;
	std::vector<Key> m_key;
	std::vector<Effect> m_effect;
	Map* m_pMap;

	bool m_render;
	float m_health, m_speed;
	bool m_updateInput;
	bool m_deathExecuted;
	bool m_playAudio;
	bool m_handleInput;
};


#endif