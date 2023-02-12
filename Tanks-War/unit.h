#ifndef _UNIT_H
#define _UNIT_H

#define UNIT_KEYS		4
#define KEYFORWARD		0
#define KEYBACK			1
#define KEYRIGHT		2
#define KEYLEFT			3
#define UNIT_EFFECTS	5
#define EFFECTFORWARD	0
#define EFFECTBACK		1
#define EFFECTRIGHT		2
#define EFFECTLEFT		3
#define EFFECTDEATH		4
#define FRAMEDEATH FRAMEEND
#define HP		// Health taken by HP
#define PX		// Speed taken by PX

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
	virtual void initialize(int width, int height, int columns, int rows, bool _animate, float _updateDelay, float _health, float _speed, Image* _death, Map* _map, TextureManger* _textureManger, Graphics* _graphics);
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
	float getSin() { return m_sinA; }
	float getCos() { return m_cosA; }
	int getSinSign() { return (m_sinA < 0) ? -1 : 1; }
	int getCosSign() { return (m_cosA < 0) ? -1 : 1; }

private:

	void mathUpdate();

protected:
	
	Audio* m_audio;
	Input* m_input;
	Image* m_death;
	std::vector<Key> m_key;
	std::vector<Effect> m_effect;
	Map* m_map;

	bool m_render;
	float m_health, m_speed;
	bool m_updateInput;
	bool m_deathExecuted;
	float m_sinA, m_cosA;
	bool m_playAudio;
	bool m_handleInput;
};


#endif