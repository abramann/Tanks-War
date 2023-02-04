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
	virtual void healthIncrease(float increase) { health += increase; }
	virtual void healthDecrease(float decrease) { health -= decrease; }
	virtual void speedIncrease(float increase) { speed += increase; }
	virtual void speedDecrease(float decrease) { speed -= decrease; }

	virtual void setHealth(float _health) { health = _health; }
	virtual void setSpeed(float _speed) { speed = _speed; }
	virtual void setUpdateInput(bool _updateInput) { updateInput = _updateInput; }

	virtual float getHealth() { return health; }
	virtual float getSpeed() { return speed; }

	virtual bool getUpdateInput() { return updateInput; }
	float getSin() { return sinA; }
	float getCos() { return cosA; }
	int getSinSign() { if (sinA > 0) return 1; if (sinA < 0)return -1; return 0; }
	int getCosSign() { if (cosA > 0) return 1; if (cosA < 0)return -1; return 0; }

private:

	void mathUpdate();

protected:
	
	Audio* audio;
	Input* input;
	Image* death;
	Key* key;
	Effect* effect;
	Map* map;

	bool render;
	float health, speed;
	bool updateInput;
	bool deathExecuted;
	float sinA, cosA;
	bool playAudio;
	bool handleInput;
};


#endif