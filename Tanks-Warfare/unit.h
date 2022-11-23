#ifndef _UNIT_H
#define _UNIT_H
#define FORWARD 0
#define BACK 1
#define RIGHT 2
#define LEFT 3
#define UNIT_HEALTH 100
#define UNIT_SPEED 10

#include "image.h"
#include "audio.h"
#include "input.h"

class Unit : public Image
{
public:

	Unit();
	~Unit();
	virtual void initialize(int _width, int _height, int extraWidth, int extraHeight, int currentFrame, int startFrame, int endFrame, float health, float speed, TextureManger* _textureManger, Graphics* _graphics);
	virtual void inputInitialize(Input* _input,KEY forward_key,KEY back_key,KEY right_key,KEY left_key);
	virtual void update(float frameTime);
	virtual void inputUpdate(float frameTime);
	virtual void executeForward(float frameTime);
	virtual void executeBack(float frameTime);
	virtual void executeRight(float frameTime);
	virtual void executeLeft(float frameTime);

	virtual void setHealth(float _health) { health = _health; }
	virtual void setSpeed(float _speed) { speed = _speed; }
	
	float getHealth() { return health; }
	float getSpeed() { return speed; }
	float getAngleSin() { return sinA; }
	float getAngleCos() { return cosA; }

private:

	void mathUpdate();

	float sinA, cosA;

protected:
	
	Audio* audio;
	Input* input;

	KEY key[4];
	float health, speed;
	bool updateInput;

};


#endif