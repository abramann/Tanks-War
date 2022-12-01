#ifndef _TANK_H
#define _TANK_H
#include "unit.h"
#include "fire.h"

#define TANK_KEYS			5
#define TANK_RECTS			3
#define TANK_RECTNORMAL		0
#define TANK_RECTATTACK		1
#define TANK_RECTDEATH		2
#define TANK_FRAMESTART		1
#define TANK_FRAMEEND		3
#define TANK_FRAMES			3
#define TANK_FRAMEATTACK	3
#define TANK_KEYS			5
#define TANK_KEYATTACK		4
#define TANK_EFFECTS		7
#define TANK_EFFECTSHOT		5
#define TANK_EFFECTHIT		6

const float TANK_ATTACKDELAY = 1000.0f;

class Tank : public Unit
{
public:

	Tank();
	~Tank();
	virtual void initialize(int width, int height, float _health, float _speed, TextureManger* _textureManger, Graphics* _graphics);
	virtual void inputInitialize(Input* _input, Key forward_key, Key back_key, Key right_key, Key left_key, Key attack_key, TextureManger* _fireTex);
	virtual void audioInitialize(Effect _forward_eff, Effect back_eff, Effect right_eff, Effect left_eff, Effect death_eff, Effect shot_eff, Effect hit_eff);
	virtual void inputUpdate(float frameTime);
	virtual void release();
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);
	virtual void executeAttack(float frameTime);
	virtual void setTexture(TextureManger* _textureManger, TextureManger* _fireTex);

protected:

	TextureManger* fireTex;
	RECT* rect;
	Fire fire;

};


#endif